## Broken y-axis
https://matplotlib.org/stable/gallery/subplots_axes_and_figures/broken_axis.html

### Idea

#### Create 2 Subplots, White Space in Between
Create 2 Axes, a top Axes and a bottom Axes, representing the top outlier part and bottom part. Set the height ratio by `gridspec_kw={'height_ratios': [1, 3]}` and also the white space in between by `hspace=0.1`.

Create Axes:
```python
import matplotlib.pyplot as plt

# create broken y-axis figure
fig, (ax_top, ax_bottom) = plt.subplots(
    2, 1, sharex=True, # broken-y shares same x
    figsize=(4.5, 6.6),
    gridspec_kw={'height_ratios': [1, 3]} 
)
plt.subplots_adjust(left=0.22, right=0.95, top=0.8, bottom=0.12, hspace=0.1)
```

#### set y axis limits
Note that we shall set these 2 plots to cover 2 different plot range (an upper one and a lower one).

```python
ax_top.set_ylim(3.8, 5.0)
ax_bottom.set_ylim(0.8, 2.7)
```

#### Hide the Boundding Box Line in Between (Spine)
Control the spines with Axes.spines to make `ax_top.spines['bottom']` and `ax_bottom.spines['top']` invisible. 

Besides, `ax_top.xaxis.tick_top()` makes the top subplot tick atop, so that the invisible x-xaxis will not have visible ticks.
Also, `ax_top.tick_params(labeltop=False)` disable the top Axes to display tick labels due to `sharex`.

```python
# hide spines between top and bottom
ax_top.spines['bottom'].set_visible(False)
ax_bottom.spines['top'].set_visible(False)
ax_top.xaxis.tick_top()
ax_top.tick_params(labeltop=False)  # no xlabels on top
ax_bottom.xaxis.tick_bottom()
```

#### Plot twice
Now the broken layout is about to finish. 

In this step, you plot your data twice both in `ax_top` and `ax_bottom`.

#### Drawing diagonal break marks (slanted lines)
```python
# add diagonal break marks
d = 0.5  # proportion of vertical to horizontal extent of the slanted line
kwargs = dict(marker=[(-1, -d), (1, d)], markersize=24,
            linestyle="none", color='k', mec='k', mew=1, clip_on=False) # marker definition
# ploting markers
ax_top.plot([0, 1], [0, 0], transform=ax_top.transAxes, **kwargs)
ax_bottom.plot([0, 1], [1, 1], transform=ax_bottom.transAxes, **kwargs)
```

`marker=[(-1, -d), (1, d)]` defines a marker shape that connecting [(-1, -d), (1, d)]. The size is control by `markersize`, which means `marker` only controls direction not the scale: [(-1, -d), (1, d)] and [(-2, -2*d), (2, 2*d)] is exactly the same.

Then, place the marker lines at 4 breaking points: `[0, 1], [0, 0]` in `transform=ax_top.transAxes` space and `[0, 1], [1, 1]` in ` transform=ax_bottom.transAxes` space, with `plt.plot`. (`linestyle="none"` means marker-only, no connecting lines.)

Congrats, your broken y-axis figure is finished!