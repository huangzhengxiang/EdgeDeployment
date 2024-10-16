""" PyTorch Qwen2 model."""
import math
import warnings
from typing import List, Optional, Tuple, Union

import torch
import torch.nn.functional as F
import torch.utils.checkpoint
from torch import nn
from torch.nn import BCEWithLogitsLoss, CrossEntropyLoss, MSELoss

from transformers.activations import ACT2FN
from transformers.cache_utils import Cache, DynamicCache
from transformers.modeling_attn_mask_utils import _prepare_4d_causal_attention_mask, _prepare_4d_causal_attention_mask_for_sdpa
from transformers.modeling_outputs import BaseModelOutputWithPast, CausalLMOutputWithPast, SequenceClassifierOutputWithPast
from transformers.modeling_utils import PreTrainedModel
from transformers.utils import (
    logging,
)
from .configuration_qwen2 import Qwen2Config




logger = logging.get_logger(__name__)


# Copied from transformers.models.llama.modeling_llama.LlamaRMSNorm with Llama->Qwen2
class Qwen2RMSNorm(nn.Module):
    def __init__(self, hidden_size, eps=1e-6):
        """
        Qwen2RMSNorm is equivalent to T5LayerNorm
        """
        super().__init__()
        self.weight = nn.Parameter(torch.ones(hidden_size))
        self.variance_epsilon = eps

    def forward(self, hidden_states):
        # fill in the blanks


# Copied from transformers.models.llama.modeling_llama.rotate_half
def rotate_half(x):
    """Rotates half the hidden dims of the input."""
    x1 = x[..., : x.shape[-1] // 2]
    x2 = x[..., x.shape[-1] // 2 :]
    return torch.cat((-x2, x1), dim=-1)


def _get_rotary_pos_emb(rope_theta,
                        position_ids, 
                        head_dim):
    # \frac{1}{theta^{\frac{2i}{head_dim}}}
    # theta: [head_dim//2]
    theta = 1.0 / (rope_theta ** (torch.arange(0, head_dim, 2, dtype=torch.float32) / head_dim))
    position_ids = position_ids.float().reshape(-1, 1)
    idx_theta = position_ids * theta # (q_len, head_dim//2)
    rotary_pos_emb = torch.cat((idx_theta, idx_theta), dim=-1) # (q_len, head_dim)
    rotary_pos_emb = rotary_pos_emb.unsqueeze(1).unsqueeze(0) # (1, q_len, 1, head_dim)
    rotary_pos_emb = torch.stack([torch.cos(rotary_pos_emb), torch.sin(rotary_pos_emb)])
    # Tuple[(1, q_len, 1, head_dim), (1, q_len, 1, head_dim)]
    return rotary_pos_emb


# Copied from transformers.models.mistral.modeling_mistral.MistralMLP with Mistral->Qwen2
class Qwen2MLP(nn.Module):
    def __init__(self, config):
        super().__init__()
        self.config = config
        self.hidden_size = config.hidden_size
        self.intermediate_size = config.intermediate_size
        self.gate_proj = nn.Linear(self.hidden_size, self.intermediate_size, bias=False)
        self.up_proj = nn.Linear(self.hidden_size, self.intermediate_size, bias=False)
        self.down_proj = nn.Linear(self.intermediate_size, self.hidden_size, bias=False)
        self.act_fn = ACT2FN[config.hidden_act]

    def forward(self, x):
        # fill in the blanks


class Qwen2Attention(nn.Module):
    """
    Multi-headed attention from 'Attention Is All You Need' paper. Modified to use sliding window attention: Longformer
    and "Generating Long Sequences with Sparse Transformers".
    """

    def __init__(self, config: Qwen2Config, layer_idx: Optional[int] = None):
        super().__init__()
        self.config = config
        self.layer_idx = layer_idx

        self.hidden_size = config.hidden_size
        self.num_heads = config.num_attention_heads
        self.head_dim = self.hidden_size // self.num_heads
        self.num_key_value_heads = config.num_key_value_heads
        self.num_key_value_groups = self.num_heads // self.num_key_value_heads
        self.max_position_embeddings = config.max_position_embeddings
        self.rope_theta = config.rope_theta
        self.is_causal = True
        self.attention_dropout = config.attention_dropout

        self.q_proj = nn.Linear(self.hidden_size, self.num_heads * self.head_dim, bias=True)
        self.k_proj = nn.Linear(self.hidden_size, self.num_key_value_heads * self.head_dim, bias=True)
        self.v_proj = nn.Linear(self.hidden_size, self.num_key_value_heads * self.head_dim, bias=True)
        self.o_proj = nn.Linear(self.num_heads * self.head_dim, self.hidden_size, bias=False)

    def forward(
        self,
        hidden_states: torch.Tensor,
        attention_mask: Optional[torch.Tensor] = None,
        position_ids: Optional[torch.LongTensor] = None,
        past_key_value: Optional[torch.Tensor] = None,
    ) -> Tuple[torch.Tensor, Tuple[torch.Tensor]]:
        
        if self.num_heads != self.num_key_value_heads:
            raise NotImplementedError("Grouped QA not implemented yet!!!")

        # fill in the blanks


class Qwen2DecoderLayer(nn.Module):
    def __init__(self, config: Qwen2Config, layer_idx: int):
        super().__init__()
        self.hidden_size = config.hidden_size
        self.self_attn = Qwen2Attention(config, layer_idx)
        self.mlp = Qwen2MLP(config)
        self.input_layernorm = Qwen2RMSNorm(config.hidden_size, eps=config.rms_norm_eps)
        self.post_attention_layernorm = Qwen2RMSNorm(config.hidden_size, eps=config.rms_norm_eps)

    def forward(
        self,
        hidden_states: torch.Tensor,
        attention_mask: Optional[torch.Tensor] = None,
        position_ids: Optional[torch.LongTensor] = None,
        past_key_value: Optional[Tuple[torch.Tensor]] = None
    ) -> Tuple[torch.FloatTensor, Optional[Tuple[torch.FloatTensor, torch.FloatTensor]]]:

        """
        Args:
            hidden_states (`torch.FloatTensor`): input to the layer of shape `(batch, seq_len, embed_dim)`
            attention_mask (`torch.FloatTensor`, *optional*): attention mask of size
                `(batch, sequence_length)` where padding elements are indicated by 0.
            past_key_value (`Tuple(torch.FloatTensor)`, *optional*): cached past key and value projection states
        """
        # inputs
        residual = hidden_states
        hidden_states = self.input_layernorm(hidden_states)

        # Self Attention
        hidden_states, present_key_value = self.self_attn(
            hidden_states=hidden_states,
            attention_mask=attention_mask,
            position_ids=position_ids,
            past_key_value=past_key_value,
        )
        hidden_states = residual + hidden_states

        # Fully Connected
        residual = hidden_states
        hidden_states = self.post_attention_layernorm(hidden_states)
        hidden_states = self.mlp(hidden_states)
        hidden_states = residual + hidden_states

        # return hidden_states and present_key_value of this layer
        outputs = (hidden_states, present_key_value)
        return outputs




class Qwen2PreTrainedModel(PreTrainedModel):
    config_class = Qwen2Config
    base_model_prefix = "model"
    supports_gradient_checkpointing = True
    _no_split_modules = ["Qwen2DecoderLayer"]
    _skip_keys_device_placement = "past_key_values"
    _supports_flash_attn_2 = True
    _supports_sdpa = True
    _supports_cache_class = True

    def _init_weights(self, module):
        std = self.config.initializer_range
        if isinstance(module, nn.Linear):
            module.weight.data.normal_(mean=0.0, std=std)
            if module.bias is not None:
                module.bias.data.zero_()
        elif isinstance(module, nn.Embedding):
            module.weight.data.normal_(mean=0.0, std=std)
            if module.padding_idx is not None:
                module.weight.data[module.padding_idx].zero_()

def _get_qwen2_attention_mask(
    query_length: int,
    past_key_values_length: int,
):
    """
    Creates a causal 4D mask of shape `(batch_size, 1, query_length, key_value_length)` from a 2D mask of shape
    `(batch_size, key_value_length)`

    Args:
        query_length (`int`):
            The length of new query.
        past_key_values_length (`int`):
            The length of the key value cache.
    """
    """only support batchsize==1, but support multi-round query"""

    attention_mask = (1 - torch.tril(torch.ones([1, 1, query_length, query_length]))) * torch.finfo(torch.float32).min
    if past_key_values_length > 0:
        attention_mask = torch.cat((torch.zeros([1, 1, query_length, past_key_values_length]), attention_mask), dim=-1)
    return attention_mask


class Qwen2Model(Qwen2PreTrainedModel):
    """
    Transformer decoder consisting of *config.num_hidden_layers* layers. Each layer is a [`Qwen2DecoderLayer`]

    Args:
        config: Qwen2Config
    """

    def __init__(self, config: Qwen2Config):
        super().__init__(config)
        self.padding_idx = config.pad_token_id
        self.vocab_size = config.vocab_size

        # initialize all layers (except for lm_head)
        self.embed_tokens = nn.Embedding(config.vocab_size, config.hidden_size, self.padding_idx)
        self.layers = nn.ModuleList(
            [Qwen2DecoderLayer(config, layer_idx) for layer_idx in range(config.num_hidden_layers)]
        )
        self._attn_implementation = config._attn_implementation
        self.norm = Qwen2RMSNorm(config.hidden_size, eps=config.rms_norm_eps)

        # Initialize weights and apply final processing
        self.post_init()

        # Initialize kv_cache
        self.past_key_values = None

    def get_input_embeddings(self):
        return self.embed_tokens

    def set_input_embeddings(self, value):
        self.embed_tokens = value

    def clear(self):
        self.past_key_values = None
        return

    def forward(
        self,
        input_ids: torch.LongTensor = None,
        position_ids: Optional[torch.LongTensor] = None,
    ) -> torch.FloatTensor:

        # retrieve input_ids
        batch_size, seq_length = input_ids.shape

        # retrieve kv_cache
        if self.past_key_values is None:
            self.past_key_values = [None for _ in range(len(self.layers))]

        # retrieve position_ids
        position_ids = position_ids.view(-1, seq_length).long()

        # start with inputs_embeds
        inputs_embeds = self.embed_tokens(input_ids)
        hidden_states = inputs_embeds

        # generate 4d attention mask, and pass it through the layers
        # [1,1,seq_length,key_value_length]
        attention_mask = _get_qwen2_attention_mask(
            seq_length,
            position_ids[0,0].item()
        )

        # inference decoder layer
        for layer_id, decoder_layer in enumerate(self.layers):
            layer_outputs = decoder_layer(
                hidden_states,
                attention_mask=attention_mask,
                position_ids=position_ids,
                past_key_value=self.past_key_values[layer_id],
            )

            hidden_states = layer_outputs[0]
            self.past_key_values[layer_id] = layer_outputs[1]

        # last_layernorm
        hidden_states = self.norm(hidden_states)
        
        # return the last hidden_state
        return hidden_states[:,-1,:].view(batch_size,-1)


class Qwen2ForCausalLMChat(Qwen2PreTrainedModel):
    """
    batch_size always 1.
    """
    _tied_weights_keys = ["lm_head.weight"]

    def __init__(self, config):
        super().__init__(config)
        self.model = Qwen2Model(config)
        self.vocab_size = config.vocab_size
        self.lm_head = nn.Linear(config.hidden_size, config.vocab_size, bias=False)

        # Initialize weights and apply final processing
        self.post_init()

        # preserved states for serving.
        self.previous_token_len = 0
        self.total_token_len = 0

    def get_input_embeddings(self):
        return self.model.embed_tokens

    def set_input_embeddings(self, value):
        self.model.embed_tokens = value

    def get_output_embeddings(self):
        return self.lm_head

    def set_output_embeddings(self, new_embeddings):
        self.lm_head = new_embeddings

    def set_decoder(self, decoder):
        self.model = decoder

    def get_decoder(self):
        return self.model
    
    def clear(self):
        self.previous_token_len = 0
        self.total_token_len = 0
        # reset past_kv_values
        self.model.clear()

    def forward(
        self,
        input_ids: torch.LongTensor = None
    ) -> torch.FloatTensor:
        if len(input_ids.shape) > 1 and input_ids.shape[0] != 1:
            raise NotImplementedError("Only batchsize==1 is supported now!!!")
        # calculate position_ids
        self.total_token_len += input_ids.shape[-1]
        position_ids = torch.arange(self.previous_token_len, 
                                   self.total_token_len,
                                   device = input_ids.device,
                                   dtype=torch.long)
        self.previous_token_len = self.total_token_len

        # inference
        hidden_states = self.model(
            input_ids=input_ids.view(1,-1),
            position_ids=position_ids.view(1,-1)
        )        
        logits = self.lm_head(hidden_states)
        logits = logits.float()
        return logits