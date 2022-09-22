/*Note: this program has been changed to incorporate nifti-files*/


/*$Header: /autofs/space/nexus_001/users/nexus-tools/cvsrepository/nifti_tools/gauss_nifti/cgauss3dd.c,v 1.1 2008/08/10 20:20:39 mtt24 Exp $*/
/*$Log: cgauss3dd.c,v $
/*Revision 1.1  2008/08/10 20:20:39  mtt24
/*revision one
/*
 * Revision 1.1  2007/04/24  05:38:14  avi
 * Initial revision
 **/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>

void gauss3dd (float *image, int *pnx, int *pny, int *pnz, float *cmppix, float *fhalf) {
/****************************************************************************************/
/*	3D Gaussian filter + differentiation (variant of cgauss3d.c)			*/
/*											*/
/*	inputs: real*4 image(nx,ny,nz)	! image to be filtered				*/
/* 		real*4 cmppix(3)	! x, y, z voxel dimensions in cm		*/
/*		real*4 fhalf		! half amplitude frequency in cycles/cm		*/
/*											*/
/*	variables modified: image	! input image overwritten			*/
/*											*/
/*	subroutines called:	FFT, REALT in FORTRAN source fftsun.f or fftsol.f	*/
/* 				FFT algorithm by Richard C. Singleton			*/
/*											*/
/*	restrictions: mixed radix FFT accepts any nx, ny, nz but 			*/
/*				nx must be divisible by 2 				*/
/****************************************************************************************/
	float		*a, *b;
	double		q, factor, f2, fx, fy, fz;
	int		i, k, n1, n2, n2ny, jndex;
	int		ix, iy, iz, nx, ny, nz, kx, ky, kz;
 	int		one = 1, negone = -1;

	nx = *pnx; ny = *pny; nz = *pnz;
	if (nx % 2) {
		fprintf (stderr, "gauss3d: nx not a multiple of 2\n");
		exit (-1);
	}
	n1 = nx/2;
	n2 = n1 + 1;
	n2ny = n2*ny;
	if (!(a = (float *) malloc (n2*ny*nz * sizeof (float)))) printf ("gauss3d: cannot allocate memory");
	if (!(b = (float *) malloc (n2*ny*nz * sizeof (float)))) printf ("gauss3d: cannot allocate memory");

	k = jndex = 0;
	for (iz = 0; iz < nz; iz++) {
	for (iy = 0; iy < ny; iy++) {
		for (i = ix = 0; ix < nx; ix += 2, i++) {
			(a + k)[i] = image[jndex++];
			(b + k)[i] = image[jndex++];
		}
		fft_   (a + k, b + k, &one, &n1, &one, &negone);
		realt_ (a + k, b + k, &one, &n1, &one, &negone);
		k += n2;
	}}
	fft_ (a, b, &nz,  &ny, &n2,   &negone);
	fft_ (a, b, &one, &nz, &n2ny, &negone);

	q = -log(2.)/((*fhalf)*(*fhalf));
	i = 0;
	for (iz = 0; iz < nz; iz++) {
		kz = (iz <= nz/2) ? iz : nz - iz;
		fz = kz/(nz*cmppix[2]);
	for (iy = 0; iy < ny; iy++) {
		ky = (iy <= ny/2) ? iy : ny - iy;
		fy = ky/(ny*cmppix[1]);
	for (ix = 0; ix < n2; ix++) {
		kx = (ix <= nx/2) ? ix : nx - ix;
		fx = kx/(nx*cmppix[0]);
		f2 = fx*fx + fy*fy + fz*fz;
		factor = sqrt(f2)*exp (q*f2);
		a[i] *= factor;
 		b[i] *= factor;
		i++;
	}}}

	fft_ (a, b, &one, &nz, &n2ny, &one);
	fft_ (a, b, &nz,  &ny, &n2,   &one);
	k = jndex = 0;
	for (iz = 0; iz < nz; iz++) {
	for (iy = 0; iy < ny; iy++) {
		realt_ (a + k, b + k, &one, &n1, &one, &one);
		fft_   (a + k, b + k, &one, &n1, &one, &one);
		for (i = ix = 0; ix < nx; ix += 2, i++) {
			image[jndex++] = (a + k)[i];
			image[jndex++] = (b + k)[i];
		}
		k += n2;
	}}

	free (a);
	free (b);
}
