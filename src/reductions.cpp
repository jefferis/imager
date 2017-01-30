#include <imager.h>
#include "wrappers_cimglist.h"
using namespace Rcpp;
using namespace cimg_library;




// [[Rcpp::export]]
NumericVector reduce_sum(List x)
{
  CImgList<double> L = sharedCImgList(x);
  CId out(L.at(0),false);
  int n = x.size();
  for (int i = 1;  i < n; i++)
    {
      out += L.at(i);
    }
  return wrap(out);
}

// [[Rcpp::export]]
NumericVector reduce_wsum(List x,NumericVector w)
{
  CImgList<double> L = sharedCImgList(x);
  CId out(L.at(0),false);
  int n = x.size();
  for (int i = 1;  i < n; i++)
    {
      out += w(i)*L.at(i);
    }
  return wrap(out);
}


// [[Rcpp::export]]
NumericVector reduce_prod(List x,int summary = 0)
{
  CImgList<double> L = sharedCImgList(x);
  CId out(L.at(0),false);
  int n = x.size();
  for (int i = 1;  i < n; i++)
    {
      out = out.mul(L.at(i));
    }
  return wrap(out);
}


// [[Rcpp::export]]
NumericVector reduce_list(List x,int summary = 0)
{
  CImgList<double> L = sharedCImgList(x);
  CId out(L.at(0),false);
  int n = x.size();
  cimg_pragma_openmp(parallel for cimg_openmp_if(out.size()>=65536))
  cimg_forXYZC(out,x,y,z,c)
    {
      CId vec(n,1,1,1);
      for (int i = 0; i <n; i++)
	{
	  vec[i] = L.at(i)(x,y,z,c);
	  //	  vec[i] = L.atNXYZC(i,x,y,z,c);
	}
      switch (summary)
	{
	case 1:
	  out(x,y,z,c) = vec.min(); break;
	case 2:
	  out(x,y,z,c) = vec.max(); break;
	case 3:
	  out(x,y,z,c) = vec.median(); break;
	case 4:
	  out(x,y,z,c) = vec.variance(); break;
	case 5:
	  out(x,y,z,c) = sqrt(vec.pow(2).sum()); break;
	}
    }
  return wrap(out);
}


//OpenMP seems not to do anything for these functions on certain platforms. Don't know why yet. 

// [[Rcpp::export]]
NumericVector reduce_list2(List x,int summary = 0)
{
  CImgList<double> L = sharedCImgList(x);
  CId out(L.at(0),false);
  int n = x.size();
  cimg_pragma_openmp(parallel for collapse(2))
  cimg_forXYZC(out,x,y,z,c)
    {
      CId vec(n,1,1,1);
      for (int i = 0; i <n; i++)
	{
	  vec[i] = L.at(i)(x,y,z,c);
	  //	  vec[i] = L.atNXYZC(i,x,y,z,c);
	}
      switch (summary)
	{
	case 1:
	  out(x,y,z,c) = vec.min(); break;
	case 2:
	  out(x,y,z,c) = vec.max(); break;
	case 3:
	  out(x,y,z,c) = vec.median(); break;
	case 4:
	  out(x,y,z,c) = vec.variance(); break;
	case 5:
	  out(x,y,z,c) = sqrt(vec.pow(2).sum()); break;
	}
    }
  return wrap(out);
}




// [[Rcpp::export]]
List psort(List x,bool increasing = true)
{
  CImgList<double> L = sharedCImgList(x);
  CImgList<double> out(L,false);
  int n = x.size();
  cimg_pragma_openmp(parallel for cimg_openmp_if(out.size()>=65536))
    cimg_forXYZC(L.at(0),x,y,z,c)
    {
      CId vec(n,1,1,1),perm(n,1,1,1);
      
      for (int i = 0; i <n; i++)
	{
	  vec[i] = L.at(i)(x,y,z,c);
	  //	  vec[i] = L.atNXYZC(i,x,y,z,c);
	}
      vec.sort(perm,increasing);
      for (int i = 0; i <n; i++)
	{
	  out.at(i)(x,y,z,c) = vec[i];
	}

    }
  return wrap(out);
}

// [[Rcpp::export]]
List porder(List x,bool increasing = true)
{
  CImgList<double> L = sharedCImgList(x);
  CImgList<double> out(L,false);
  int n = x.size();
  cimg_pragma_openmp(parallel for cimg_openmp_if(out.size()>=65536))
    cimg_forXYZC(L.at(0),x,y,z,c)
    {
      CId vec(n,1,1,1),perm(n,1,1,1);
      
      for (int i = 0; i <n; i++)
	{
	  vec[i] = L.at(i)(x,y,z,c);
	  //	  vec[i] = L.atNXYZC(i,x,y,z,c);
	}
      vec.sort(perm,increasing);
      for (int i = 0; i <n; i++)
	{
	  out.at(i)(x,y,z,c) = perm[i] + 1;
	}

    }
  return wrap(out);
}


// [[Rcpp::export]]
List prank(List x,bool increasing = true)
{
  CImgList<double> L = sharedCImgList(x);
  CImgList<double> out(L,false);
  int n = x.size();
  cimg_pragma_openmp(parallel for cimg_openmp_if(out.size()>=65536))
    cimg_forXYZC(L.at(0),x,y,z,c)
    {
      CId vec(n,1,1,1),perm(n,1,1,1);
      
      for (int i = 0; i <n; i++)
	{
	  vec[i] = L.at(i)(x,y,z,c);
	  //	  vec[i] = L.atNXYZC(i,x,y,z,c);
	}
      vec.sort(perm,increasing);
      for (int i = 0; i <n; i++)
	{
	  out.at(perm(i))(x,y,z,c) = i + 1;
	}

    }
  return wrap(out);
}
