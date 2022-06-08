//   Copyright 2022 Will Thomas
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef _IMG_IMG_H
#define _IMG_IMG_H

#include <stdint.h>

typedef struct img_s {
	uint8_t*** pix;
	uint32_t w;
	uint32_t h;
} img_t;

img_t* img_init(uint32_t, uint32_t);

img_t* img_init_col(uint32_t, uint32_t, uint8_t, uint8_t, uint8_t);

img_t* img_init_pix(uint32_t, uint32_t, uint8_t*, uint8_t, uint8_t);

img_t* img_resz(img_t*, uint32_t, uint32_t);

void img_clr(img_t*);

#endif