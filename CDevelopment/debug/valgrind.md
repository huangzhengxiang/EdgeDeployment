## Valgrind

### quick start
https://valgrind.org/docs/manual/quick-start.html


### massif
visualize and analyze heap use.
https://valgrind.org/docs/manual/ms-manual.html

```bash
valgrind --tool=massif prog
ms_print massif.out.12345
```