/* matrix/minmax_source.c
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Gerard Jungman, Brian Gough
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

BASE
FUNCTION (gsl_matrix, max) (const TYPE (gsl_matrix) * m)
{
  /* finds the largest element of a matrix */

  const size_t M = m->size1;
  const size_t N = m->size2;
  const size_t tda = m->tda;

  BASE max = m->data[0 * tda + 0];
  size_t i, j;

  for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
        {
          BASE x = m->data[i * tda + j];
          if (x > max)
            max = x;
        }
    }

  return max;
}

BASE
FUNCTION (gsl_matrix, min) (const TYPE (gsl_matrix) * m)
{
  /* finds the smallest element of a matrix */

  const size_t M = m->size1;
  const size_t N = m->size2;
  const size_t tda = m->tda;

  BASE min = m->data[0 * tda + 0];
  size_t i, j;

  for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
        {
          BASE x = m->data[i * tda + j];
          if (x < min)
            min = x;
        }
    }

  return min;
}


void
FUNCTION (gsl_matrix, minmax) (const TYPE (gsl_matrix) * m,
                               BASE * min_out,
                               BASE * max_out)
{
  /* finds the smallest and largest elements of a matrix */

  const size_t M = m->size1;
  const size_t N = m->size2;
  const size_t tda = m->tda;

  BASE max = m->data[0 * tda + 0];
  BASE min = m->data[0 * tda + 0];

  size_t i, j;

  for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
        {
          BASE x = m->data[i * tda + j];
          if (x < min)
            {
              min = x;
            }
          if (x > max)
            {
              max = x;
            }
        }
    }

  *min_out = min;
  *max_out = max;
}

void
FUNCTION (gsl_matrix, max_index) (const TYPE (gsl_matrix) * m, size_t * imax_out, size_t *jmax_out)
{
  /* finds the largest element of a matrix */

  const size_t M = m->size1;
  const size_t N = m->size2;
  const size_t tda = m->tda;

  BASE max = m->data[0 * tda + 0];
  size_t imax = 0, jmax = 0;
  size_t i, j;

  for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
        {
          BASE x = m->data[i * tda + j];
          if (x > max)
            {
              max = x;
              imax = i;
              jmax = j;
            }
        }
    }

  *imax_out = imax;
  *jmax_out = jmax;
}

void
FUNCTION (gsl_matrix, min_index) (const TYPE (gsl_matrix) * m, size_t * imin_out, size_t *jmin_out)
{
  /* finds the largest element of a matrix */

  const size_t M = m->size1;
  const size_t N = m->size2;
  const size_t tda = m->tda;

  BASE min = m->data[0 * tda + 0];
  size_t imin = 0, jmin = 0;
  size_t i, j;

  for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
        {
          BASE x = m->data[i * tda + j];
          if (x < min)
            {
              min = x;
              imin = i;
              jmin = j;
            }
        }
    }

  *imin_out = imin;
  *jmin_out = jmin;
}

void
FUNCTION (gsl_matrix, minmax_index) (const TYPE (gsl_matrix) * m,
                                     size_t * imin_out,
                                     size_t * jmin_out,
                                     size_t * imax_out,
                                     size_t * jmax_out)
{
  /* finds the smallest and largest elements of a matrix */

  const size_t M = m->size1;
  const size_t N = m->size2;
  const size_t tda = m->tda;

  size_t imin = 0, jmin = 0, imax = 0, jmax = 0;
  BASE max = m->data[0 * tda + 0];
  BASE min = m->data[0 * tda + 0];

  size_t i, j;

  for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
        {
          BASE x = m->data[i * tda + j];
          if (x < min)
            {
              min = x;
              imin = i;
              jmin = j;
            }
          if (x > max)
            {
              max = x;
              imax = i;
              jmax = j;
            }
        }
    }

  *imin_out = imin;
  *jmin_out = jmin;
  *imax_out = imax;
  *jmax_out = jmax;
}
