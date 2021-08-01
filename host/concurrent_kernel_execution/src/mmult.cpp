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

#define MAX_DIM 160
#define PARALLEL 160

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
//hls::vector<unsigned int, PARALLEL>* a
static void compute(hls::vector<unsigned int, PARALLEL> & in1,
                        hls::vector<unsigned int, PARALLEL> & in2,
                        hls::stream<hls::vector<unsigned int, PARALLEL> >& out_stream,
                        int vSize) {
// The kernel is operating with SIMD vectors of PARALLEL integers. The + operator performs
// an element-wise add, resulting in PARALLEL parallel additions.

	hls::vector<int, PARALLEL> local_in1(vSize);
	hls::vector<int, PARALLEL> local_in2(vSize);
        hls::vector<unsigned int, PARALLEL> tmp_arr(1); // one entry
mem_rd:
    for (int i = 0; i < vSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
	    local_in1[i] = in1[i];
	    local_in2[i] = in2[i];
    }

mmult1:
    for (int j = 0; j < dim1; ++j) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
    mmult2:
        for (int i = 0; i < dim0; ++i) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
	    //Assume the second matrix (local_in2) is reversed
	    tmp_arr = local_in1[j] * local_in2[i];
        add3:
            int temp = 0;
	    int * arr temp_arr.data();
            for (int k = 0; k < dim1; ++k)
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
	    	temp += arr[k];

	    out_stream << temp;
            //c[i + j * dim0] = temp;
        }
    }
}

extern "C" {
#if 0
void mmult(int* c, int* a, const int* b, const int dim0, const int dim1) {
    int matA[MAX_DIM * MAX_DIM];
    int matB[MAX_DIM * MAX_DIM];

// Auto-pipeline is going to apply pipeline to these loops
mmult_readA:
    for (int i = 0; i < dim0 * dim1; ++i) {
#pragma HLS LOOP_TRIPCOUNT min = c_size* c_size max = c_size * c_size
        matA[i] = a[i];
    }

mmult_readB:
    for (int i = 0; i < dim0 * dim1; ++i) {
#pragma HLS LOOP_TRIPCOUNT min = c_size* c_size max = c_size * c_size
        matB[i] = b[i];
    }

mmult1:
    for (int j = 0; j < dim1; ++j) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
    mmult2:
        for (int i = 0; i < dim0; ++i) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
            int temp = 0;
        mmult3:
            for (int k = 0; k < dim1; ++k)
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
                temp += matA[k + i * dim0] * matB[j + k * dim0];

            c[i + j * dim0] = temp;
        }
    }
}
#else
void mmult(hls::vector<unsigned int, PARALLEL>* c,
          hls::vector<unsigned int, PARALLEL>* a,
          hls::vector<unsigned int, PARALLEL>* b,
	  const int dim0,
	  const int dim1) 
{
#pragma HLS INTERFACE m_axi port = c bundle = gmem1
#pragma HLS INTERFACE m_axi port = a bundle = gmem0
#pragma HLS INTERFACE m_axi port = b bundle = gmem0
    static hls::stream<hls::vector<unsigned int, PARALLEL> > in1_stream("input_stream_1");
    static hls::stream<hls::vector<unsigned int, PARALLEL> > in2_stream("input_stream_2");
    static hls::stream<hls::vector<unsigned int, PARALLEL> > out_stream("output_stream");

    assert(dim0*dim1/PARALLEL == 0);
    int vSize = dim0*dim1/PARALLEL;
#pragma HLS dataflow

#if 0
    load_input(a, in1_stream, vSize);
    load_input(b, in2_stream, vSize);
#endif

    compute(a, b, out_stream, vSize);
    store_result(c, out_sream, vSize);

}
#endif
}
