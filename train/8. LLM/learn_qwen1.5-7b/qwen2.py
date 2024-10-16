import os
import base64
import glob
import json
import shutil
import argparse
import torch
import numpy as np
import sentencepiece as spm
from transformers import AutoModel, AutoModelForCausalLM, AutoTokenizer


class Qwen2_Chat(torch.nn.Module):
    def __init__(self, args):
        super().__init__()
        self.stop_ids = []
        self.total_token_len = 0
        self.max_length = 4096
        self.load_model(args.path)

    def load_pretrained(self, model_path: str):
        self.tokenizer = AutoTokenizer.from_pretrained(model_path, trust_remote_code=True)
        self.model = AutoModelForCausalLM.from_pretrained(model_path, trust_remote_code=True).float().eval()
        self.config = self.model.config

    def load_model(self, model_path: str):
        # Qwen2 models
        self.model_name = 'Qwen2'
        self.load_pretrained(model_path)
        # some wrapper
        self.stop_ids.append(self.tokenizer.eos_token_id)
        if hasattr(self.model, 'generation_config') and hasattr(self.model.generation_config, "eos_token_id"):
            for id in self.model.generation_config.eos_token_id:
                self.stop_ids.append(id)

    def forward(self, input_ids):
        return self.model(input_ids)

    def str_to_ids(self, prompt):
        input_ids = self.tokenizer(prompt, return_tensors="pt")['input_ids']
        return input_ids

    def id_to_str(self, token_id):
        word = self.tokenizer._convert_id_to_token(int(token_id))
        word = self.tokenizer.convert_tokens_to_string([word])
        return word

    def build_prompt(self, query):
        return f'<|im_start|>user\n{query}<|im_end|>\n<|im_start|>assistant\n'

    def clear(self):
        self.total_token_len = 0
        # reset token length
        self.model.clear()

    def response(self, query):
        prompt = self.build_prompt(query)
        input_ids = self.str_to_ids(prompt)
        # start this round of response
        token_id = input_ids
        self.total_token_len += input_ids.shape[-1]
        while self.total_token_len < self.max_length:
            logits = self.forward(token_id.reshape(1, -1))
            token_id = torch.argmax(logits)
            self.total_token_len += 1
            if token_id in self.stop_ids:
                print("", end='\n')
                break
            word = self.id_to_str(token_id)
            print(word, end="", flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='llm_exporter', formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('--path', type=str, default='THUDM/chatglm-6b', required=True,
                        help='path(`str` or `os.PathLike`):\nCan be either:'
                        '\n\t- A string, the *model id* of a pretrained model like `THUDM/chatglm-6b`. [TODO]'
                        '\n\t- A path to a *directory* clone from repo like `../chatglm-6b`.')
    parser.add_argument('--test', type=str, help='test model inference with query `TEST`.')


    args = parser.parse_args()
    model_path = args.path

    # copy modeling py file to pretrain model for export
    for file in glob.glob(f'./Qwen1_5-7B-Chat/*'):
        shutil.copy2(file, model_path)

    llm = Qwen2_Chat(args)

    # chatbot
    if args.test is not None:
        test_strs = args.test.split("\n")
        for test_str in test_strs:
            llm.response(test_str)
            break