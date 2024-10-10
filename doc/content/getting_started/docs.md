# Locally build the documentation

Often times, it is useful to locally host the documentation of RACCOON (this website), both for convenience and for development purposes. To build the documentation, first make sure you have compiled RACCOON. Then build the documentation using

```bash
cd doc
./moosedocs.py build --serve
```

When you see messages like:

```bash
[I 211002 16:01:21 server:335] Serving on http://127.0.0.1:8000
[I 211002 16:01:21 handlers:62] Start watching changes
[I 211002 16:01:21 handlers:64] Start detecting changes
```

your local documentation is ready to be viewed. Simply open a browser and navigate to [`http://127.0.0.1:8000`](http://127.0.0.1:8000), and you will see this website in live on your own computer.

If you would like to make changes to the documentation for development purposes, checkout the official [MooseDown documentation](https://mooseframework.inl.gov/python/MooseDocs/specification.html).
