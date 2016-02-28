#pragma once

#include <GL/glew.h>
#include <vector>

class gpu_profiler
{
public:
  void init()
  {
#ifdef GPU_PROFILER_ENABLED
    query_ids.resize( initial_size );
    query_values.resize( query_ids.size() );
    glGenQueries( initial_size, &query_ids[0] );
    current_query = 0;
    query_in_progress = false;
    got_results = false;
#endif
  }

  unsigned startQuery( GLenum m )
  {
#ifdef GPU_PROFILER_ENABLED
    assert( m == GL_SAMPLES_PASSED || m == GL_TIME_ELAPSED || m == GL_PRIMITIVES_GENERATED );
    mode = m;
    query_in_progress = true;
    glBeginQuery( mode, query_ids[current_query] );
    return current_query;
#else
    return 0xDEADBEEF;
#endif
  }

  void endQuery()
  {
#ifdef GPU_PROFILER_ENABLED
    glEndQuery( mode );
    query_in_progress = false;
    current_query++;

    if( current_query > query_ids.size() - 1 )
    {
      query_ids.resize( query_ids.size() + initial_size );
      query_values.resize( query_ids.size( ) );
      glGenQueries( initial_size, &query_ids[current_query] );
    }
#endif
  }

  void resetAtEndOfFrame()
  {
#ifdef GPU_PROFILER_ENABLED
    assert( !query_in_progress );

    current_query = 0;
    got_results = false;
#endif
  }

  void getResults()
  {
#ifdef GPU_PROFILER_ENABLED
    assert( !query_in_progress );

    for( unsigned c = 0; c < current_query; ++c )
    {
      GLint done = 0;
      while( !done )
      {
        glGetQueryObjectiv( query_ids[c], GL_QUERY_RESULT_AVAILABLE, &done );
      }

      glGetQueryObjectui64v( query_ids[c], GL_QUERY_RESULT, &query_values[c] );
    }

    got_results = true;
#endif
  }

  GLuint64 getQueryResult( unsigned q )
  {
#ifdef GPU_PROFILER_ENABLED
    assert( !query_in_progress );
    assert( got_results );

    return query_values[q];
#else
    return 0;
#endif
  }
protected:
private:
  static const int initial_size = 100;

  bool query_in_progress;
  bool got_results;
  GLenum mode;
  unsigned current_query;

  vector<GLuint> query_ids;
  vector<GLuint64> query_values;
};
