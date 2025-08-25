## Figure Layout

Outside the major plot, the layout of legend, title, label, ticks, axes are also important and easily affect the visual effects of the the whole figure.

### Legend

#### Gather Handles and Remove Duplications

Gather handles method1: `Axes.get_legend_handles_labels()`,
```python
handles, labels = axes[0].get_legend_handles_labels()
```

Gather handles method2: manually gather each plot handle, e.g.,
```python
rects = axes[n].bar(pos, 
                    var_list.values(), 
                    width=width,
                    label=var_list.keys(),
                    color=colors)
[handles.append(rect) for rect in rects]
```

Filter duplications by label: check if the handle's label is already in label list,
```python
if not (var in all_labels):
    all_handles.append(rects[0])   # one rectangle per bar group is enough
    all_labels.append(var)
```

You can also remap the labels or reorder them after you gather all handles and labels. (Remember that the elements in handles list and label list shall correspond to each other.)

#### Legend Layout Settings
After the handles and corresponding labels are ready, input them to the initializer of legend as in the code below.
To fit the legend into the figure, we may also dynamically adjust the legend layout:

```python
# set 
legend = fig.legend(all_handles,         # legend handles
                    all_labels,          # legend labels
                    loc=(0.02, 0.82),    # legend location
                    fontsize=label_size, # legend text fontsize
                    ncol=4,              # column number of legends
                    # spacing
                    handlelength=2.0,    # length of the legend handles
                    handletextpad=1.0,   # space between handle and text
                    columnspacing=0.95,  # horizontal spacing between entries
                    labelspacing=0.8,    # vertical space between rows
                    borderaxespad=0.5,   # padding between legend and axes
                    borderpad=0.5        # padding inside legend box
)

# set the legend transparency
legend.get_frame().set_alpha(1) # not transparent
```