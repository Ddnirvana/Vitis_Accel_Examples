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

// Tripcount identifiers
const int c_size = MAX_DIM;

//Add
extern "C" {
void host_madd(int* c, int* a, int* b, const int dim0, const int dim1) {
    int matA[MAX_DIM * MAX_DIM];
    int matB[MAX_DIM * MAX_DIM];

madd_readA:
    for (int i = 0; i < dim0 * dim1; ++i) {
        matA[i] = a[i];
    }

madd_readB:
    for (int i = 0; i < dim0 * dim1; ++i) {
        matB[i] = b[i];
    }

madd_writeC:
    for (int i = 0; i < dim0 * dim1; ++i) {
        c[i] = matA[i] + matB[i];
    }
}
}


//Multi
extern "C" {
void host_mmult(int* c, int* a, const int* b, const int dim0, const int dim1) {
    int matA[MAX_DIM * MAX_DIM];
    int matB[MAX_DIM * MAX_DIM];

mmult_readA:
    for (int i = 0; i < dim0 * dim1; ++i) {
        matA[i] = a[i];
    }

mmult_readB:
    for (int i = 0; i < dim0 * dim1; ++i) {
        matB[i] = b[i];
    }

mmult1:
    for (int j = 0; j < dim1; ++j) {
    mmult2:
        for (int i = 0; i < dim0; ++i) {
            int temp = 0;
        mmult3:
            for (int k = 0; k < dim1; ++k)
                temp += matA[k + i * dim0] * matB[j + k * dim0];

            c[i + j * dim0] = temp;
        }
    }
}
}

//Scaling
extern "C" {
void host_mscale(int* inout_r, const int scale, const int dim0, const int dim1) {
    int temp[MAX_DIM * MAX_DIM];

// Auto-pipeline is going to apply pipeline to these loops
mscale:
    for (int i = 0; i < dim0 * dim1; ++i)
        temp[i] = inout_r[i] * scale;

mscale_write:
    for (int i = 0; i < dim0 * dim1; ++i)
        inout_r[i] = temp[i];
}
}
