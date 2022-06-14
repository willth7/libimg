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
//	optimize image type differentiation

#include <img/img.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void bmp_write(img_t* img, char* path) {
	if (img->type == IMG_R8G8B8) {
		uint8_t p = 4 - (img->w * 3) % 4;
		if (p == 4) p = 0;
		uint32_t fsz = (img->w * 3 + p) * img->h + 54;
		uint32_t psz = (img->w * 3 + p) * img->h;
		FILE* f = fopen(path, "w");
		fprintf(f, "%c%c", 66, 77); //signature
		fprintf(f, "%c%c%c%c", (char) fsz & 255, (char) (fsz >> 8) & 255, (char) (fsz >> 16) & 255, (char) (fsz >> 24) & 255); //file size
		fprintf(f, "%c%c%c%c", 0, 0, 0, 0); //reserved
		fprintf(f, "%c%c%c%c", 54, 0, 0, 0); //pixel array offset
		fprintf(f, "%c%c%c%c", 40, 0, 0, 0); //DIB header size
		fprintf(f, "%c%c%c%c", (char) img->w & 255, (char) (img->w >> 8) & 255, (char) (img->w >> 16) & 255, (char) (img->w >> 24) & 255); //width
		fprintf(f, "%c%c%c%c", (char) img->h & 255, (char) (img->h >> 8) & 255, (char) (img->h >> 16) & 255, (char) (img->h >> 24) & 255); //height
		fprintf(f, "%c%c", 1, 0); //colors panes
		fprintf(f, "%c%c", 24, 0); //bits per pixel
		fprintf(f, "%c%c%c%c", 0, 0, 0, 0); //compression
		fprintf(f, "%c%c%c%c", (char) psz & 255, (char) (psz >> 8) & 255, (char) (psz >> 16) & 255, (char) (psz >> 24) & 255); //pixel array size
		fprintf(f, "%c%c%c%c", 35, 46, 0, 0); //horizontal resolution
		fprintf(f, "%c%c%c%c", 35, 46, 0, 0); //vertical resolution
		fprintf(f, "%c%c%c%c%c%c%c%c", 0, 0, 0, 0, 0, 0, 0, 0); //junk
		for (uint32_t hi = 0; hi < img->h; hi++) {
			for (uint32_t wi = 0; wi < img->w; wi++) {
				fprintf(f, "%c", *(img->pix + (hi * img->w * 3) + (wi * 3) + 2)); //blue
				fprintf(f, "%c", *(img->pix + (hi * img->w * 3) + (wi * 3) + 1)); //green
				fprintf(f, "%c", *(img->pix + (hi * img->w * 3) + (wi * 3))); //red
			}
			for (uint8_t wi = 0; wi < p; wi++) fprintf(f, "%c", 0); //padding
		}
		fclose(f);
	}
	else if (img->type == IMG_R8G8B8A8) {
		uint32_t fsz = (img->w * 4) * img->h + 70;
		uint32_t psz = (img->w * 4) * img->h;
		FILE* f = fopen(path, "w");
		fprintf(f, "%c%c", 66, 77); //signature
		fprintf(f, "%c%c%c%c", (char) fsz & 255, (char) (fsz >> 8) & 255, (char) (fsz >> 16) & 255, (char) (fsz >> 24) & 255); //file size
		fprintf(f, "%c%c%c%c", 0, 0, 0, 0); //reserved
		fprintf(f, "%c%c%c%c", 70, 0, 0, 0); //pixel array offset
		fprintf(f, "%c%c%c%c", 56, 0, 0, 0); //DIB header size
		fprintf(f, "%c%c%c%c", (char) img->w & 255, (char) (img->w >> 8) & 255, (char) (img->w >> 16) & 255, (char) (img->w >> 24) & 255); //width
		fprintf(f, "%c%c%c%c", (char) img->h & 255, (char) (img->h >> 8) & 255, (char) (img->h >> 16) & 255, (char) (img->h >> 24) & 255); //height
		fprintf(f, "%c%c", 1, 0); //colors panes
		fprintf(f, "%c%c", 32, 0); //bits per pixel
		fprintf(f, "%c%c%c%c", 0, 0, 0, 0); //compression
		fprintf(f, "%c%c%c%c", (char) psz & 255, (char) (psz >> 8) & 255, (char) (psz >> 16) & 255, (char) (psz >> 24) & 255); //pixel array size
		fprintf(f, "%c%c%c%c", 35, 46, 0, 0); //horizontal resolution
		fprintf(f, "%c%c%c%c", 35, 46, 0, 0); //vertical resolution
		fprintf(f, "%c%c%c%c%c%c%c%c", 0, 0, 0, 0, 0, 0, 0, 0); //junk
		fprintf(f, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 0, 0, 0, 0, 255); //alpha bit mask
		for (uint32_t hi = 0; hi < img->h; hi++) {
			for (uint32_t wi = 0; wi < img->w; wi++) {
				fprintf(f, "%c", *(img->pix + (hi * img->w * 4) + (wi * 4) + 2)); //blue
				fprintf(f, "%c", *(img->pix + (hi * img->w * 4) + (wi * 4) + 1)); //green
				fprintf(f, "%c", *(img->pix + (hi * img->w * 4) + (wi * 4))); //red
				fprintf(f, "%c", *(img->pix + (hi * img->w * 4) + (wi * 4) + 3)); //alpha
			}
		}
		fclose(f);
	}
}

img_t* bmp_read(char* path) {
	FILE* f = fopen(path, "r");
	if (f == NULL) return NULL;
	fseek(f, 0, SEEK_END);
	uint64_t bytesz = ftell(f);
	uint8_t* data = malloc(bytesz);
	fseek(f, 0, SEEK_SET);
	fread(data, bytesz, 1, f);
	fclose(f);
	if (*data != 66 || *(data + 1) != 77) return NULL;
	uint8_t off = *(data + 10) + (*(data + 11) << 8) + (*(data + 12) << 16) + (*(data + 13) << 24);
	uint32_t w = *(data + 18) + (*(data + 19) << 8) + (*(data + 20) << 16) + (*(data + 21) << 24);
	uint32_t h = *(data + 22) + (*(data + 23) << 8) + (*(data + 24) << 16) + (*(data + 25) << 24);
	uint16_t bpp = *(data + 28) + (*(data + 29) << 8);
	if (bpp == 24) {
		uint8_t p = 4 - (w * 3) % 4;
		if (p == 4) p = 0;
		img_t* img = malloc(sizeof(img_t));
		img = img_init_raw(IMG_R8G8B8, w, h, data, off, p);
		free(data);
		return img;
	}
	else if (bpp == 32) {
		img_t* img = malloc(sizeof(img_t));
		img = img_init_raw(IMG_R8G8B8A8, w, h, data, off, 0);
		free(data);
		return img;
	}
	return NULL;
}
