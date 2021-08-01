/**
* Copyright (C) 2020 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

#include <hls_vector.h>
#include <hls_stream.h>
#include "assert.h"

#define MAX_DIM 160
#define PARALLEL 64

// Tripcount identifiers
const int c_size = MAX_DIM;

static void store_result(hls::vector<unsigned int, PARALLEL>* out,
                         hls::stream<hls::vector<unsigned int, PARALLEL> >& out_stream,
                         int vSize) {
mem_wr:
    for (int i = 0; i < vSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        out[i] = out_stream.read();
    }
}

extern "C" {
#if 0
void mscale(int* inout_r, const int scale, const int dim0, const int dim1) {
    int temp[MAX_DIM * MAX_DIM];

// Auto-pipeline is going to apply pipeline to these loops
mscale:
    for (int i = 0; i < dim0 * dim1; ++i)
#pragma HLS LOOP_TRIPCOUNT min = c_size* c_size max = c_size * c_size
        temp[i] = inout_r[i] * scale;

mscale_write:
    for (int i = 0; i < dim0 * dim1; ++i)
#pragma HLS LOOP_TRIPCOUNT min = c_size* c_size max = c_size * c_size
        inout_r[i] = temp[i];
}
#else

void mscale(hls::vector<unsigned int, PARALLEL>* inout_r,
		const int scale,
	  const int dim0,
	  const int dim1) 
{
#pragma HLS INTERFACE m_axi port = inout_r bundle = gmem0
    //static hls::stream<hls::vector<unsigned int, PARALLEL> > in_stream("input_stream_1");
    static hls::stream<hls::vector<unsigned int, PARALLEL> > out_stream("output_stream");

    assert(dim0*dim1%PARALLEL == 0);
    int vSize = dim0*dim1/PARALLEL;
#pragma HLS dataflow

    for (int i = 0; i < vSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        out_stream << (inout_r[i] * scale);
    }

    store_result(inout_r, out_stream, vSize);

}

#endif
}
