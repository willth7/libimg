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

#include <stdint.h>
#include <stdlib.h>

typedef struct img_s {
	uint8_t* pix;
	uint8_t type;
	uint32_t w;
	uint32_t h;
} img_t;

img_t* img_init(uint8_t type, uint32_t w, uint32_t h) {
	if (type == 0) {
		img_t* img = malloc(sizeof(img_t));
		img->pix = malloc(24 * w * h);
		img->w = w;
		img->h = h;
		img->type = type;
		return img;
	}
	else if (type == 1) {
		img_t* img = malloc(sizeof(img_t));
		img->pix = malloc(32 * w * h);
		img->w = w;
		img->h = h;
		img->type = type;
		return img;
	}
	return 0;
}

img_t* img_init_raw(uint8_t type, uint32_t w, uint32_t h, uint8_t* data) {
	if (type == 0) {
		img_t* img = malloc(sizeof(img_t));
		img->pix = malloc(24 * w * h);
		uint8_t p = 4 - (w * 3) % 4;
		if (p == 4) {
			p = 0;
		}
		uint32_t indx = 0;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(img->pix + (hi * w * 3) + (wi * 3)) = *(data + indx + 2);
				*(img->pix + (hi * w * 3) + (wi * 3) + 1) = *(data + indx + 1);
				*(img->pix + (hi * w * 3) + (wi * 3) + 2) = *(data + indx);
				indx += 3;
			}
			indx += p;
		}
		img->type = type;
		img->w = w;
		img->h = h;
		return img;
	}
	else if (type == 1) {
		img_t* img = malloc(sizeof(img_t));
		img->pix = malloc(32 * w * h);
		uint32_t indx = 0;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(img->pix + (hi * w * 4) + (wi * 4)) = *(data + indx + 2);
				*(img->pix + (hi * w * 4) + (wi * 4) + 1) = *(data + indx + 1);
				*(img->pix + (hi * w * 4) + (wi * 4) + 2) = *(data + indx);
				*(img->pix + (hi * w * 4) + (wi * 4) + 3) = *(data + indx + 3);
				indx += 4;
			}
		}
		img->type = type;
		img->w = w;
		img->h = h;
		return img;
	}
	return 0;
}

void img_clr(img_t* img) {
	free(img->pix);
	free(img);
}

void img_resz(img_t* img, uint32_t w, uint32_t h) {
	if (img->type == 0) {
		uint8_t* pix = malloc(24 * w * h);
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				uint64_t r = 0;
				uint64_t g = 0;
				uint64_t b = 0;
				uint64_t cnt = 0;
				for (uint32_t hj = (img->h * hi) / h; hj <= (img->h * (hi + 1)) / h; hj++) {
					for (uint32_t wj = (img->w * wi) / w; wj <= (img->w * (wi + 1)) / w; wj++) {
						if (wj < img->w && hj < img->h) {
							r += *(img->pix + (hj * img->w * 3) + (wj * 3));
							g += *(img->pix + (hj * img->w * 3) + (wj * 3) + 1);
							b += *(img->pix + (hj * img->w * 3) + (wj * 3) + 2);
							cnt++;
						}
					}
				}
				*(pix + (hi * w * 3) + (wi * 3)) = (uint8_t) (r / cnt);
				*(pix + (hi * w * 3) + (wi * 3) + 1) = (uint8_t) (g / cnt);
				*(pix + (hi * w * 3) + (wi * 3) + 2) = (uint8_t) (b / cnt);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
	else if (img->type == 1) {
		uint8_t* pix = malloc(32 * w * h);
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				uint64_t r = 0;
				uint64_t g = 0;
				uint64_t b = 0;
				uint64_t a = 0;
				uint64_t cnt = 0;
				for (uint32_t hj = (img->h * hi) / h; hj <= (img->h * (hi + 1)) / h; hj++) {
					for (uint32_t wj = (img->w * wi) / w; wj <= (img->w * (wi + 1)) / w; wj++) {
						if (wj < img->w && hj < img->h) {
							r += *(img->pix + (hj * img->w * 4) + (wj * 4));
							g += *(img->pix + (hj * img->w * 4) + (wj * 4) + 1);
							b += *(img->pix + (hj * img->w * 4) + (wj * 4) + 2);
							a += *(img->pix + (hj * img->w * 4) + (wj * 4) + 3);
							cnt++;
						}
					}
				}
				*(pix + (hi * w * 4) + (wi * 4)) = (uint8_t) (r / cnt);
				*(pix + (hi * w * 4) + (wi * 4) + 1) = (uint8_t) (g / cnt);
				*(pix + (hi * w * 4) + (wi * 4) + 2) = (uint8_t) (b / cnt);
				*(pix + (hi * w * 4) + (wi * 4) + 3) = (uint8_t) (a / cnt);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
}

void img_crop(img_t* img, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
	if (img->type == 0) {
		if ((x + w > img->w) || (y + h > img->h)) {
			return;
		}
		uint8_t* pix = malloc(24 * w * h);
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(pix + (hi * w * 3) + (wi * 3)) = *(img->pix + ((hi + y) * img->w * 3) + ((wi + x) * 3));
				*(pix + (hi * w * 3) + (wi * 3) + 1) = *(img->pix + ((hi + y) * img->w * 3) + ((wi + x) * 3) + 1);
				*(pix + (hi * w * 3) + (wi * 3) + 2) = *(img->pix + ((hi + y) * img->w * 3) + ((wi + x) * 3) + 2);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
	else if (img->type == 1) {
		if ((x + w > img->w) || (y + h > img->h)) {
			return;
		}
		uint8_t* pix = malloc(32 * w * h);
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(pix + (hi * w * 4) + (wi * 4)) = *(img->pix + ((hi + y) * img->w * 4) + ((wi + x) * 4));
				*(pix + (hi * w * 4) + (wi * 4) + 1) = *(img->pix + ((hi + y) * img->w * 4) + ((wi + x) * 4) + 1);
				*(pix + (hi * w * 4) + (wi * 4) + 2) = *(img->pix + ((hi + y) * img->w * 4) + ((wi + x) * 4) + 2);
				*(pix + (hi * w * 4) + (wi * 4) + 3) = *(img->pix + ((hi + y) * img->w * 4) + ((wi + x) * 4) + 3);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
}

void img_flip_h(img_t* img) {
	if (img->type == 0) {
		uint8_t* pix = malloc(24 * img->w * img->h);
		for (uint32_t hi = 0; hi < img->h; hi++) {
			for (uint32_t wi = 0; wi < img->w; wi++) {
				*(pix + (hi * img->w * 3) + (wi * 3)) = *(img->pix + ((img->h - (hi + 1)) * img->w * 3) + (wi * 3));
				*(pix + (hi * img->w * 3) + (wi * 3) + 1) = *(img->pix + ((img->h - (hi + 1)) * img->w * 3) + (wi * 3) + 1);
				*(pix + (hi * img->w * 3) + (wi * 3) + 2) = *(img->pix + ((img->h - (hi + 1)) * img->w * 3) + (wi * 3) + 2);
			}
		}
		free(img->pix);
		img->pix = pix;
	}
	else if (img->type == 1) {
		uint8_t* pix = malloc(32 * img->w * img->h);
		for (uint32_t hi = 0; hi < img->h; hi++) {
			for (uint32_t wi = 0; wi < img->w; wi++) {
				*(pix + (hi * img->w * 4) + (wi * 4)) = *(img->pix + ((img->h - (hi + 1)) * img->w * 4) + (wi * 4));
				*(pix + (hi * img->w * 4) + (wi * 4) + 1) = *(img->pix + ((img->h - (hi + 1)) * img->w * 4) + (wi * 4) + 1);
				*(pix + (hi * img->w * 4) + (wi * 4) + 2) = *(img->pix + ((img->h - (hi + 1)) * img->w * 4) + (wi * 4) + 2);
				*(pix + (hi * img->w * 4) + (wi * 4) + 3) = *(img->pix + ((img->h - (hi + 1)) * img->w * 4) + (wi * 4) + 3);
			}
		}
		free(img->pix);
		img->pix = pix;
	}
}

void img_flip_v(img_t* img) {
	if (img->type == 0) {
		uint8_t* pix = malloc(24 * img->w * img->h);
		for (uint32_t hi = 0; hi < img->h; hi++) {
			for (uint32_t wi = 0; wi < img->w; wi++) {
				*(pix + (hi * img->w * 3) + (wi * 3)) = *(img->pix + (hi * img->w * 3) + ((img->w - (wi + 1)) * 3));
				*(pix + (hi * img->w * 3) + (wi * 3) + 1) = *(img->pix + (hi * img->w * 3) + ((img->w - (wi + 1)) * 3) + 1);
				*(pix + (hi * img->w * 3) + (wi * 3) + 2) = *(img->pix + (hi * img->w * 3) + ((img->w - (wi + 1)) * 3) + 2);
			}
		}
		free(img->pix);
		img->pix = pix;
	}
	else if (img->type == 1) {
		uint8_t* pix = malloc(32 * img->w * img->h);
		for (uint32_t hi = 0; hi < img->h; hi++) {
			for (uint32_t wi = 0; wi < img->w; wi++) {
				*(pix + (hi * img->w * 4) + (wi * 4)) = *(img->pix + (hi * img->w * 4) + ((img->w - (wi + 1)) * 4));
				*(pix + (hi * img->w * 4) + (wi * 4) + 1) = *(img->pix + (hi * img->w * 4) + ((img->w - (wi + 1)) * 4) + 1);
				*(pix + (hi * img->w * 4) + (wi * 4) + 2) = *(img->pix + (hi * img->w * 4) + ((img->w - (wi + 1)) * 4) + 2);
				*(pix + (hi * img->w * 4) + (wi * 4) + 3) = *(img->pix + (hi * img->w * 4) + ((img->w - (wi + 1)) * 4) + 3);
			}
		}
		free(img->pix);
		img->pix = pix;
	}
}

void img_rot_cw(img_t* img) {
	if (img->type == 0) {
		uint8_t* pix = malloc(24 * img->w * img->h);
		uint32_t w = img->h;
		uint32_t h = img->w;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(pix + (hi * w * 3) + (wi * 3)) = *(img->pix + (wi * img->w * 3) + ((img->w - (hi + 1)) * 3));
				*(pix + (hi * w * 3) + (wi * 3) + 1) = *(img->pix + (wi * img->w * 3) + ((img->w - (hi + 1)) * 3) + 1);
				*(pix + (hi * w * 3) + (wi * 3) + 2) = *(img->pix + (wi * img->w * 3) + ((img->w - (hi + 1)) * 3) + 2);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
	else if (img->type == 1) {
		uint8_t* pix = malloc(32 * img->w * img->h);
		uint32_t w = img->h;
		uint32_t h = img->w;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(pix + (hi * w * 4) + (wi * 4)) = *(img->pix + (wi * img->w * 4) + ((img->w - (hi + 1)) * 4));
				*(pix + (hi * w * 4) + (wi * 4) + 1) = *(img->pix + (wi * img->w * 4) + ((img->w - (hi + 1)) * 4) + 1);
				*(pix + (hi * w * 4) + (wi * 4) + 2) = *(img->pix + (wi * img->w * 4) + ((img->w - (hi + 1)) * 4) + 2);
				*(pix + (hi * w * 4) + (wi * 4) + 3) = *(img->pix + (wi * img->w * 4) + ((img->w - (hi + 1)) * 4) + 3);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
}

void img_rot_ccw(img_t* img) {
	if (img->type == 0) {
		uint8_t* pix = malloc(24 * img->w * img->h);
		uint32_t w = img->h;
		uint32_t h = img->w;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(pix + (hi * w * 3) + (wi * 3)) = *(img->pix + ((img->h - (wi + 1)) * img->w * 3) + (hi * 3));
				*(pix + (hi * w * 3) + (wi * 3) + 1) = *(img->pix + ((img->h - (wi + 1)) * img->w * 3) + (hi * 3) + 1);
				*(pix + (hi * w * 3) + (wi * 3) + 2) = *(img->pix + ((img->h - (wi + 1)) * img->w * 3) + (hi * 3) + 2);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
	else if (img->type == 1) {
		uint8_t* pix = malloc(32 * img->w * img->h);
		uint32_t w = img->h;
		uint32_t h = img->w;
		for (uint32_t hi = 0; hi < h; hi++) {
			for (uint32_t wi = 0; wi < w; wi++) {
				*(pix + (hi * w * 4) + (wi * 4)) = *(img->pix + ((img->h - (wi + 1)) * img->w * 4) + (hi * 4));
				*(pix + (hi * w * 4) + (wi * 4) + 1) = *(img->pix + ((img->h - (wi + 1)) * img->w * 4) + (hi * 4) + 1);
				*(pix + (hi * w * 4) + (wi * 4) + 2) = *(img->pix + ((img->h - (wi + 1)) * img->w * 4) + (hi * 4) + 2);
				*(pix + (hi * w * 4) + (wi * 4) + 3) = *(img->pix + ((img->h - (wi + 1)) * img->w * 4) + (hi * 4) + 3);
			}
		}
		free(img->pix);
		img->pix = pix;
		img->w = w;
		img->h = h;
	}
}

void img_inv(img_t* img) {
	if (img->type == 0) {
		for (uint32_t h = 0; h < img->h; h++) {
			for (uint32_t w = 0; w < img->w; w++) {
				*(img->pix + (h * img->w * 3) + (w * 3)) = 255 - *(img->pix + (h * img->w * 3) + (w * 3));
				*(img->pix + (h * img->w * 3) + (w * 3) + 1) = 255 - *(img->pix + (h * img->w * 3) + (w * 3) + 1);
				*(img->pix + (h * img->w * 3) + (w * 3) + 2) = 255 - *(img->pix + (h * img->w * 3) + (w * 3) + 2);
			}
		}
	}
	else if (img->type == 1) {
		for (uint32_t h = 0; h < img->h; h++) {
			for (uint32_t w = 0; w < img->w; w++) {
				*(img->pix + (h * img->w * 4) + (w * 4)) = 255 - *(img->pix + (h * img->w * 4) + (w * 4));
				*(img->pix + (h * img->w * 4) + (w * 4) + 1) = 255 - *(img->pix + (h * img->w * 4) + (w * 4) + 1);
				*(img->pix + (h * img->w * 4) + (w * 4) + 2) = 255 - *(img->pix + (h * img->w * 4) + (w * 4) + 2);
			}
		}
	}
}
