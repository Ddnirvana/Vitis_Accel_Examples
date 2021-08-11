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

extern "C" {

//Function list
//
//1
void madd2(int* c, int* a, const int* b, 
	  const int dim0,
	  const int dim1);
//2
void madd3(int* c, int* a, const int* b,
	  const int dim0,
	  const int dim1);
//3
void madd4(int* c, int* a, const int* b, 
	  const int dim0,
	  const int dim1);
//4
void mscale2(hls::vector<unsigned int, PARALLEL>* inout_r,
		const int scale,
	  const int dim0,
	  const int dim1);
//5
void mscale3(hls::vector<unsigned int, PARALLEL>* inout_r,
		const int scale,
	  const int dim0,
	  const int dim1);
//6
void mscale4(hls::vector<unsigned int, PARALLEL>* inout_r,
		const int scale,
	  const int dim0,
	  const int dim1);
//7
void mmult2(int* c, int* a, const int* b, const int dim0, const int dim1);
//8
void mmult3(int* c, int* a, const int* b, const int dim0, const int dim1);
//9
void mmult4(int* c, int* a, const int* b, const int dim0, const int dim1);
//10
void madd5(int* c, int* a, const int* b, 
	  const int dim0,
	  const int dim1);
//11
void mscale5(hls::vector<unsigned int, PARALLEL>* inout_r,
		const int scale,
	  const int dim0,
	  const int dim1);
//12
void mmult5(int* c, int* a, const int* b, const int dim0, const int dim1);

/* 
 * This is a wrapper to dispatch functions,
 * The first argument is always the function_no,
 * The remaining arguments are big set of all included functions
 * */
//function_handler_wrapper
void funcwrapper(
	  int func_no,
          hls::vector<unsigned int, PARALLEL>* c,
          hls::vector<unsigned int, PARALLEL>* a,
          hls::vector<unsigned int, PARALLEL>* b,
	  const int scale,
	  const int dim0,
	  const int dim1
	  ) 
{
#if 0
	switch (func_no) {
		case 1:
			return madd2(c,a,b,dim0,dim1);
		case 2:
			return madd3(c,a,b,dim0,dim1);
		case 3:
			return madd4(c,a,b,dim0,dim1);
		case 4:
			return mscale2(c,scale,dim0,dim1);
		case 5:
			return mscale3(c,scale,dim0,dim1);
		case 6:
			return mscale4(c,scale,dim0,dim1);
		case 7:
			return mmult2((int*)c, (int*)a, (int*)b,dim0,dim1);
		case 8:
			return mmult3((int*)c, (int*)a, (int*)b,dim0,dim1);
		case 9:
			return mmult4((int*)c, (int*)a, (int*)b,dim0,dim1);
	}
#else
	if (func_no ==1)
			return madd2((int*)c,(int*)a, (int*)b,dim0,dim1);
	else if (func_no == 2)
			return madd3((int*)c, (int*)a, (int*)b,dim0,dim1);
	else if (func_no == 3)
			return madd4((int*)c, (int*)a, (int*)b,dim0,dim1);
	else if (func_no == 4)
			return mscale2(c,scale,dim0,dim1);
	else if (func_no == 5)
			return mscale3(c,scale,dim0,dim1);
	else if (func_no == 6)
			return mscale4(c,scale,dim0,dim1);
	else if (func_no == 7)
			return mmult2((int*)c, (int*)a, (int*)b,dim0,dim1);
	else if (func_no == 8)
			return mmult3((int*)c, (int*)a, (int*)b,dim0,dim1);
	else if (func_no == 9)
			return mmult4((int*)c, (int*)a, (int*)b,dim0,dim1);
	else if (func_no == 10)
			return madd5((int*)c, (int*)a, (int*)b,dim0,dim1);
	else if (func_no == 11)
			return mscale5(c,scale,dim0,dim1);
	else if (func_no == 12)
			return mmult5((int*)c, (int*)a, (int*)b,dim0,dim1);
#endif
}

}
