# OpenGLGPUProfiler
## Usage:
### Initialization

```
gpu_profiler profiler;
profiler.init();
```

### Use this to query elapsed GPU time in nanosec

```
auto query_id = profiler.startQuery( GL_TIME_ELAPSED );
```

### Or use this to query the number of samples that got shaded

```
auto query_id = profiler.startQuery( GL_SAMPLES_PASSED );
```

### Or use this to query the number of primitives (triangles, points, lines) generated

```
auto query_id = profiler.startQuery( GL_PRIMITIVES_GENERATED );
```

### Do rendering commands between startQuery()'s and endQuery()'s

```
//
//...
//Run rendering commands you want to profile
//...
//
```

### Mark the end of measurement for the last query with endQuery()

```
profiler.endQuery();
```

### Make sure you swap your buffers before getting the results back

```
swapBuffers();
```

### This command asks OpenGL for the query results

```
profiler.getResults();
```

### You can get a particular query's result with getQueryResult()

```
auto result = profiler.getQueryResult( query_id );
```

### If you used GL_TIME_ELAPSED, use this to get elapsed time in milliseconds

```
float time_elapsed = result * 0.001 * 0.001;
```
