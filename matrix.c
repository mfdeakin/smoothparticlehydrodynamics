
#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#ifdef MTXDEBUG
#define debug(str) fprintf(stderr, str)
#define fdebug fprintf
#else
int debug(char *str) {
	return 0;
}
#endif

#if MTXMAJORORDER == MTXMAJORROW
#define MTXMAJORFUNC matrixPosRow
#else
#define MTXMAJORFUNC matrixPosCol
#endif

struct matrix {
	float *mtx;
	unsigned width, height;
    unsigned (*matrixPos)(struct matrix *mtx, unsigned x, unsigned y);
};

/* Calculates the position in the matrix array of the value */
unsigned matrixPosCol(struct matrix *mtx, unsigned x, unsigned y)
{
    assert(mtx && x < mtx->width && y < mtx->height);
    /* For column major order: */
    return mtx->height * x + y;
}

unsigned matrixPosRow(struct matrix *mtx, unsigned x, unsigned y)
{
    assert(mtx && x < mtx->width && y < mtx->height);
    /* For row major order: */
    return mtx->width * y + x;
}

struct matrix *mtxCreate(unsigned width, unsigned height)
{
	if(!width || !height) {
		debug("mtxCreate: Width and height are zero\n");
		return NULL;
	}
	struct matrix *mtx = malloc(sizeof(struct matrix));
    assert(mtx);
    mtx->mtx = malloc(sizeof(float[width][height]));
    assert(mtx->mtx);
    memset(mtx->mtx, 0.0, sizeof(float[width][height]));
	mtx->width = width;
	mtx->height = height;
    mtx->matrixPos = MTXMAJORFUNC;
	return mtx;
}

struct matrix *mtxCreateI(unsigned size)
{
	if(!size) {
		debug("mtxCreateI: size is zero\n");
		return NULL;
	}
	struct matrix *mtx = malloc(sizeof(struct matrix));
    assert(mtx);
	mtx->mtx = malloc(sizeof(float[size][size]));
    assert(mtx->mtx);
	memset(mtx->mtx, 0.0f, sizeof(float[size][size]));
	for(int i = 0; i < 9; i += 4)
		mtx->mtx[i] = 1.0f;
	mtx->width = size;
	mtx->height = size;
	return mtx;
}

struct matrix *mtxCopy(struct matrix *mtx)
{
    assert(mtx);
	struct matrix *copy = mtxCreate(mtx->width, mtx->height);
	int i;
	for(i = 0; i < (mtx->width * mtx->height); i++)
		copy->mtx[i] = mtx->mtx[i];
	return copy;
}

struct matrix *mtxFromArray(float array[], unsigned w, unsigned h)
{
    assert(array);
	struct matrix *mtx = mtxCreate(w, h);
    unsigned i, x, y;
    for(i = 0, x = 0, y = 0;
        i < w * h;
        i++, x++) {
        if(x == mtx->width) {
            x = 0;
            y++;
        }
        unsigned pos = mtx->matrixPos(mtx, x, y);
        mtx->mtx[pos] = array[i];
    }
	return mtx;
}

void mtxFree(struct matrix *mtx)
{
    assert(mtx && mtx->mtx);
	free(mtx->mtx);
	free(mtx);
}

void mtxSetOrder(struct matrix *mtx, enum matrixorder order)
{
    assert(mtx &&
           (order == MTXROWMAJORORDER ||
            order == MTXCOLMAJORORDER));
    if(order == MTXROWMAJORORDER)
        mtx->matrixPos = matrixPosRow;
    else
        mtx->matrixPos = matrixPosCol;
}

struct matrix *mtxAdd(struct matrix *lhs, struct matrix *rhs)
{
    assert(lhs && rhs);
	if(lhs->width != rhs->width || lhs->height != rhs->height) {
		debug("mtxAdd: lhs and rhs are not valid for adding\n");
		return NULL;
	}
	struct matrix *mtx = mtxCreate(lhs->width, lhs->height);
    unsigned i, x, y;
    for(i = 0; i < (lhs->width * lhs->height); i++) {
        if(x == lhs->width) {
            x = 0;
            y++;
        }
        unsigned pos = lhs->matrixPos(lhs, x, y);
        mtx->mtx[pos] = lhs->mtx[pos] + rhs->mtx[pos];
    }
	return mtx;
}

struct matrix *mtxSub(struct matrix *lhs, struct matrix *rhs)
{
    assert(lhs && rhs);
	if(lhs->width != rhs->width || lhs->height != rhs->height) {
		debug("mtxSub: lhs and rhs are not valid for subtracting\n");
		return NULL;
	}
	struct matrix *mtx = mtxCreate(lhs->width, lhs->height);
    unsigned i, x, y;
    for(i = 0; i < (lhs->width * lhs->height); i++) {
        if(x == lhs->width) {
            x = 0;
            y++;
        }
        unsigned pos = lhs->matrixPos(lhs, x, y);
        mtx->mtx[pos] = lhs->mtx[pos] - rhs->mtx[pos];
    }
    return mtx;
}

struct matrix *mtxNeg(struct matrix *mtx)
{
    assert(mtx);
	struct matrix *ret = mtxCreate(mtx->width, mtx->height);
	int i;
	for(i = 0; i < (mtx->width * mtx->height); i++)
		ret->mtx[i] = -mtx->mtx[i];
	return ret;
}

struct matrix *mtxMul(struct matrix *lhs, struct matrix *rhs)
{
    assert(lhs && rhs);
	if(lhs->width != rhs->height) {
		debug("mtxMul: lhs and rhs cannot be multiplied\n");
		return NULL;
	}
	struct matrix *mtx = mtxCreate(rhs->width, lhs->height);
	unsigned x = 0, y = 0;
    for(unsigned i = 0; i < (mtx->width * mtx->height); i++, x++) {
		if(x == mtx->width) {
			x = 0;
			y++;
		}
        unsigned outpos = mtx->matrixPos(mtx, x, y);
        for(unsigned xypos = 0; xypos < lhs->width; xypos++) {
            unsigned lhspos = lhs->matrixPos(lhs, xypos, y),
                    rhspos = rhs->matrixPos(rhs, x, xypos);
            mtx->mtx[outpos] += lhs->mtx[lhspos] * rhs->mtx[rhspos];
        }
	}
	return mtx;
}

float mtxDeterminant(struct matrix *mtx)
{
    assert(mtx);
	if(mtx->width != mtx->height) {
		debug("mtxDeterminate: Not a square matrix\n");
        /* Return NaN */
        return 0.0 / 0.0;
	}
    if(mtx->width == 1) {
		return mtx->mtx[0];
    }
    if(mtx->width == 2) {
        unsigned pos0 = mtx->matrixPos(mtx, 0, 0),
                pos1 = mtx->matrixPos(mtx, 1, 1),
                pos2 = mtx->matrixPos(mtx, 1, 0),
                pos3 = mtx->matrixPos(mtx, 0, 1);
        return mtx->mtx[pos0] * mtx->mtx[pos1] -
                mtx->mtx[pos2] * mtx->mtx[pos3];
    }
    struct matrix *buffer = mtxCreate(mtx->width - 1, mtx->height - 1);
    float determinant = 0;
    signed sign = 1;
    for(unsigned i = 0; i < mtx->width; i++) {
        unsigned outOffset = 0;
        for(unsigned pos = 0; pos < mtx->width; pos++) {
            if(pos == i)
                continue;
            unsigned inOffset = mtx->matrixPos(mtx, pos, 1);
            for(unsigned j = 1;
                j < mtx->width;
                j++, inOffset++, outOffset++) {
                buffer->mtx[outOffset] = mtx->mtx[inOffset];
            }
        }
        determinant += sign * mtx->mtx[i] * mtxDeterminant(buffer);
        sign = -sign;
    }
    return determinant;
}

float mtxGet(struct matrix *mtx, unsigned x, unsigned y)
{
	if(x >= mtx->width || y >= mtx->height) {
		debug("mtxGet: Invalid index\n");
		return 0.0;
	}
    unsigned pos = mtx->matrixPos(mtx, x, y);
	return mtx->mtx[pos];
}

int mtxSet(struct matrix *mtx, unsigned x, unsigned y, float val)
{
	if(x >= mtx->width || y >= mtx->height) {
		debug("mtxSet: Invalid index\n");
		return -1;
	}
    unsigned pos = mtx->matrixPos(mtx, x, y);
	mtx->mtx[pos] = val;
	return 0;
}

struct matrix *mtxRotation(float theta, float x, float y, float z)
{
	float crot = cos(theta), srot = sin(theta);
	float values[16] = {
		crot + x * x * (1 - crot), x * y * (1 - crot) - z * srot, x * z * (1 - crot) + y * srot, 0.0f,
		y * x * (1 - crot) + z * srot, crot + y * y * (1 - crot), y * z * (1 - crot) - x * srot, 0.0f,
		z * x * (1 - crot) - y * srot, z * y * (1 - crot) + x * srot, crot + z * z * (1 - crot), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return mtxFromArray(values, 4, 4);
}

struct matrix *mtxTranslate(float x, float y, float z)
{
	float values[16] = {
		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return mtxFromArray(values, 4, 4);
}

struct matrix *mtxScale(float x, float y, float z)
{
	float values[16] = {
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return mtxFromArray(values, 4, 4);
}
