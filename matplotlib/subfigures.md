## Subfigures

Describes the layout of multiple subfigures.

### subplots
The rows, columns, and total figure size of subplots can be set when create by `plt.subplots`. `left`, `top`, `bottom`, `right` controls the distance between the figure area to the margin, while `wspace` and `hspace` control the spaces bewteen each subplots horizontally `wspace` and vertically `hspace`.

```python
fig, axes = plt.subplots(nrows=1, ncols=num, figsize=(num*2,3.8), sharex="none") 
plt.subplots_adjust(left=0.06, top=0.76, bottom=0.21, right=0.98, wspace=0.39, hspace=0.1)
```

The disadvantage of `subplots` is the spaces in between can only be uniform, and that's why we need `GridSpec`.

### GridSpec

The `subplots` API set equal margins between each subfigure. If customized subfigure sets are required, then `gridspec` API is definitely your first choice.

The following code spinnet: 
```python
import matplotlib
import matplotlib.gridspec as gridspec
import matplotlib.pyplot as plt

# Adjust figsize as needed
fig = plt.figure(figsize=(num*2, 3.8))  
outer = gridspec.GridSpec(1, 2, fig, 
                            left=0.07, top=0.76, bottom=0.21, right=0.98, wspace=0.18 if y=="energy" else 0.08,
                            width_ratios=[5, 2]) 
gs1 = gridspec.GridSpecFromSubplotSpec(1, 5, subplot_spec = outer[0], wspace=0.46 if y=="energy" else 0.35, width_ratios=None)
gs2 = gridspec.GridSpecFromSubplotSpec(1, 2, subplot_spec = outer[1], wspace=0.4 if y=="energy" else 0.35, width_ratios=None)
axes = [plt.subplot(cell) for cell in itertools.chain(gs1,gs2)]
```

is a more flexible and also seamlessly plug-and-use alternative of simple subplots:
```python
# Adjust figsize as needed
fig, axes = plt.subplots(nrows=1, ncols=num, figsize=(num*2,3.8), sharex="none")  
plt.subplots_adjust(left=0.06, top=0.76, bottom=0.21, right=0.98, wspace=0.39)
```