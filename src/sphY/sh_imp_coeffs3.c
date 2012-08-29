#include <stdlib.h>
#include <math.h>
#include "m_pd.h"

//#define SHLIB

#include "legendre_a.h"
#include "sh_normalization.h"
#include "sh.h"


static t_class *sh_imp_coeffs3_class;

typedef struct _sh_imp_coeffs3
{
  t_object x_obj;
  sh* dummy;
  sh* shp;
  int n;


} t_sh_imp_coeffs3;



void sh_imp_coeffs3_delete(t_sh_imp_coeffs3 *sh_imp)
{
    if (sh_imp->shp!=0)
    {
     deleteSH(sh_imp->shp);
    }
  

}


void *sh_imp_coeffs3_new(t_float f)
{
    sh* shp;
    int n=(int)f;
    t_sh_imp_coeffs3 *sh_imp=(t_sh_imp_coeffs3 *) pd_new(sh_imp_coeffs3_class);
    post("order %d",n);

    n=(n<0)?0:n;
    if ((shp=newSH(n))==0) {
	   return 0;
    }
    sh_imp->n=n;

    outlet_new(&sh_imp->x_obj,gensym("list"));
    sh_imp->shp=shp;
    sh_imp->dummy=shp;
    return (void *)sh_imp;
}


void impcoeffs_calc(t_sh_imp_coeffs3 *sh_imp, t_symbol *s, int argc, t_atom *argv)
{
    int n,m,mn;
    int len=(sh_imp->n+1)*(sh_imp->n+1);
    double phi,theta,value;
    t_atom *coeffsp;
    coeffsp=(t_atom *) calloc (len,sizeof(t_atom));
    if(argc==2)
    {
        phi=(double)atom_getfloat(argv);
        theta=(double)atom_getfloat(argv+1);
        //post("phi=%f theta=%f",phi,theta);

        for(n=0;n<=sh_imp->n;n++)
        {
            for (m=-n;m<=n;m++)
            {
                mn=(n)*(n)+n+m;
                value=sHEvaluate(sh_imp->shp,n,m,phi,theta);
                //post("m=%i n=%i value=%f",m,n,value);
                SETFLOAT(&coeffsp[mn],(float)value);
            }
        }
       outlet_list(sh_imp->x_obj.ob_outlet, gensym("list"), len,coeffsp);
    }

    else post("invalid number of input arguments");
}


void sh_imp_coeffs3_setup(void)
{
    sh_imp_coeffs3_class = class_new(gensym("sh_imp"),
                            (t_newmethod) sh_imp_coeffs3_new,
                            (t_method) sh_imp_coeffs3_delete,
                            sizeof(t_sh_imp_coeffs3),
                            CLASS_DEFAULT, A_DEFFLOAT,
                            0);

    class_addlist(sh_imp_coeffs3_class, (t_method) impcoeffs_calc);
}



