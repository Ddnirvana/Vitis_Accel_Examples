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

static void load_input(hls::vector<unsigned int, PARALLEL>* in,
                       hls::stream<hls::vector<unsigned int, PARALLEL> >& inStream,
                       int vSize) {
mem_rd:
    for (int i = 0; i < vSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        inStream << in[i];
    }
}

static void store_result(hls::vector<unsigned int, PARALLEL>* out,
                         hls::stream<hls::vector<unsigned int, PARALLEL> >& out_stream,
                         int vSize) {
mem_wr:
    for (int i = 0; i < vSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        out[i] = out_stream.read();
    }
}

static void compute(hls::stream<hls::vector<unsigned int, PARALLEL> >& in1_stream,
                        hls::stream<hls::vector<unsigned int, PARALLEL> >& in2_stream,
                        hls::stream<hls::vector<unsigned int, PARALLEL> >& out_stream,
                        int vSize) {
// The kernel is operating with SIMD vectors of PARALLEL integers. The + operator performs
// an element-wise add, resulting in PARALLEL parallel additions.
execute:
    for (int i = 0; i < vSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        out_stream << (in1_stream.read() + in2_stream.read());
    }
}

extern "C" {
#if 0
void madd(int* c, int* a, int* b, const int dim0, const int dim1) {
    int matA[MAX_DIM * MAX_DIM];
    int matB[MAX_DIM * MAX_DIM];

// Auto-pipeline is going to apply pipeline to these loops
madd_readA:
    for (int i = 0; i < dim0 * dim1; ++i) {
#pragma HLS LOOP_TRIPCOUNT min = c_size* c_size max = c_size * c_size
        matA[i] = a[i];
    }

madd_readB:
    for (int i = 0; i < dim0 * dim1; ++i) {
#pragma HLS LOOP_TRIPCOUNT min = c_size* c_size max = c_size * c_size
        matB[i] = b[i];
    }

madd_writeC:
    for (int i = 0; i < dim0 * dim1; ++i) {
#pragma HLS LOOP_TRIPCOUNT min = c_size* c_size max = c_size * c_size
        c[i] = matA[i] + matB[i];
    }
}
#else

void madd(hls::vector<unsigned int, PARALLEL>* c,
          hls::vector<unsigned int, PARALLEL>* a,
          hls::vector<unsigned int, PARALLEL>* b,
	  const int dim0,
	  const int dim1) 
{
//#pragma HLS INTERFACE m_axi port = c bundle = gmem0
//#pragma HLS INTERFACE m_axi port = a bundle = gmem0
//#pragma HLS INTERFACE m_axi port = b bundle = gmem1
    static hls::stream<hls::vector<unsigned int, PARALLEL> > in1_stream("input_stream_1");
    static hls::stream<hls::vector<unsigned int, PARALLEL> > in2_stream("input_stream_2");
    static hls::stream<hls::vector<unsigned int, PARALLEL> > out_stream("output_stream");

    assert(dim0*dim1%PARALLEL == 0);
    int vSize = dim0*dim1/PARALLEL;
#pragma HLS dataflow

    load_input(a, in1_stream, vSize);
    load_input(b, in2_stream, vSize);

    compute(in1_stream, in2_stream, out_stream, vSize);
    store_result(c, out_stream, vSize);

}
#endif
}
