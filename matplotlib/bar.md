## Bar Chart

### 1. Vertical Bar `bar`

```python
rects = axes[aid].bar(device_geo_mean[device].keys(), 
                        device_geo_mean[device].values(), 
                        width=width,
                        label=[engine_dict[e] for e in device_geo_mean[device].keys()],
                        color=[colors[engine_order.index(e)] for e in device_geo_mean[device].keys()])
axes[aid].bar_label(rects, fmt="%.1f", padding=3, fontsize=14 if mode=="mnn" else 13)
```


### 2. Horizontal Bar `barh`



### 3. Stacked Bars (Overlapping bars to make breakdown analysis)

```python
left = np.zeros(len(systems))          # running start point
colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']

for comp_idx, (comp, color) in enumerate(zip(components, colors)):
    ax.barh(y, latency[:, comp_idx], left=left,
            color=color, label=comp, edgecolor='black', linewidth=.5)
    left += latency[:, comp_idx]       # advance start for next slice
```
