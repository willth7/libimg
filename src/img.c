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

//TODO
//	color manipulation functions

#include <stdint.h>
#include <stdlib.h>

typedef struct img_s {
	uint8_t*** pix;
	uint32_t w;
	uint32_t h;
} img_t;

img_t* img_init(uint32_t w, uint32_t h) {
	img_t* img = malloc(sizeof(img_t));
	img->w = w;
	img->h = h;
	img->pix = malloc(8 * h);
	for (uint32_t hi = 0; hi < h; hi++) {
		img->pix[hi] = malloc(8 * w);
		for (uint32_t wi = 0; wi < w; wi++) {
			img->pix[hi][wi] = calloc(3, 1);
		}
	}
	return img;
}

img_t* img_init_col(uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b) {
	img_t* img = malloc(sizeof(img_t));
	img->w = w;
	img->h = h;
	img->pix = malloc(8 * h);
	for (uint32_t hi = 0; hi < h; hi++) {
		img->pix[hi] = malloc(8 * w);
		for (uint32_t wi = 0; wi < w; wi++) {
			img->pix[hi][wi] = malloc(3);
			img->pix[hi][wi][0] = r;
			img->pix[hi][wi][1] = g;
			img->pix[hi][wi][2] = b;
		}
	}
	return img;
}

img_t* img_init_pix(uint32_t w, uint32_t h, uint8_t* data, uint8_t off, uint8_t p) {
	img_t* img = malloc(sizeof(img_t));
	img->w = w;
	img->h = h;
	img->pix = malloc(8 * h);
	uint32_t indx = off;
	for (uint32_t hi = 0; hi < h; hi++) {
		img->pix[hi] = malloc(8 * w);
		for (uint32_t wi = 0; wi < w; wi++) {
			img->pix[hi][wi] = malloc(3);
			img->pix[hi][wi][0] = data[indx + 2];
			img->pix[hi][wi][1] = data[indx + 1];
			img->pix[hi][wi][2] = data[indx];
			indx += 3;
		}
		indx += p;
	}
	return img;
}

img_t* img_resz(img_t* img, uint32_t w, uint32_t h) {
	uint8_t*** pix = malloc(8 * h);
	for (uint32_t hi = 0; hi < h; hi++) {
		pix[hi] = malloc(8 * w);
		for (uint32_t wi = 0; wi < w; wi++) {
			uint64_t r = 0;
			uint64_t g = 0;
			uint64_t b = 0;
			uint64_t cnt = 0;
			for (uint32_t hj = (img->h * hi) / h; hj <= (img->h * (hi + 1)) / h; hj++) {
				for (uint32_t wj = (img->w * wi) / w; wj <= (img->w * (wi + 1)) / w; wj++) {
					if (wj < img->w && hj < img->h) {
						r += img->pix[hj][wj][0];
						g += img->pix[hj][wj][1];
						b += img->pix[hj][wj][2];
						cnt++;
					}
				}
			}
			pix[hi][wi] = malloc(3);
			pix[hi][wi][0] = (uint8_t) (r / cnt);
			pix[hi][wi][1] = (uint8_t) (g / cnt);
			pix[hi][wi][2] = (uint8_t) (b / cnt);
		}
	}
	free(img->pix);
	img->pix = pix;
	img->w = w;
	img->h = h;
	return img;
}

void img_clr(img_t* img) {
	for (uint32_t hi = 0; hi < img->h; hi++) {
		for (uint32_t wi = 0; wi < img->w; wi++) free(img->pix[hi][wi]);
		free(img->pix[hi]);
	}
	free(img->pix);
	free(img);
}
