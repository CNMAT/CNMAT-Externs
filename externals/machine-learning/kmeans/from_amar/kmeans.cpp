

#include <tcl.h>
#include <tk.h>

#include <iostream>
#include <vector>
#include <list>
#include <cmath>

#ifdef _WINDOWS
#define __export _declspec(dllexport)
#endif

using namespace std;


struct Point {
  float  x,y;
  
  Point (float ax = 0.0f, float ay = 0.0f) {
    x = ax;
    y = ay;
  }

  float distance (Point &pt) {
    float dx = x - pt.x;
    float dy = y - pt.y;
    //return dx * dx + dy * dy;
    return fabs(dx) + fabs(dy);
  }

  bool operator == (const Point &toCompare)  {
    return x == toCompare.x && y == toCompare.y;
  }
  bool operator != (const Point &toCompare) {
    return x != toCompare.x || y != toCompare.y;
  }

};

class KMeans {

public:

  KMeans (int ak, int amaxx = 14, int amaxy = 9) :
    k(ak),
    maxx(amaxx),
    maxy(amaxy),
    centroids(ak),
    amplitudes(ak,-1.0f),
    age(ak),
    coordmap(amaxx,vector<float>(amaxy,1000000.0f)),
    centroidmap(amaxx,vector<int>(amaxy,-1)) {
      xused = 0;
  }

  void Init () {
    for (int i = 0; i < k; ++i) {
      amplitudes[i] = 0.0f;
      centroids[i] = Point (rand() % maxx, rand() % maxy);
      age[i] = 0;
    }
    xused = k;
  }

  void Run (vector<Point> &coords, vector<float> &weights, int n, int hack) {
    
    vector<Point> old_centroids(k);
    vector<bool>  inPreviousFrame(k,false);
    for (int i = 0; i < k; ++i) {
      if (amplitudes[i] < 0.0f) {
	if (xused < n) {
	  //switch (rand() % 4) {
	  //case 0 : centroids[i] = Point(0,rand() % maxy); break;
	  //case 1 : centroids[i] = Point(maxx,rand() % maxy); break;
	  //case 2 : centroids[i] = Point(rand() % maxx,0); break;
	  //case 3 : centroids[i] = Point(rand() % maxx,maxy); break;
	  //}
	  //cout << "Init point " << i << ": " << centroids[i].x << ' ' 
	  //     << centroids[i].y << endl;
	  centroids[i] = Point(rand() & maxx, rand() & maxy);
	  amplitudes[i] = 0.0f;
	  age[i] = 0;
	  ++xused;
	  
	}
      } else {
	inPreviousFrame[i] = true;
      }
    }
    
    vector<list <int> > bins(k);
    bool go_again = false;
    int maxruns = k;
    
    do {
      
      for (int i = 0; i < k; ++i) {
	old_centroids[i] = centroids[i];
      }
      
      for (i = 0; i < coords.size(); ++i) {
	float mindistance = 100000.0f;
	int closest = -1;
	for (int j = 0; j < k; ++j) {
	  if (amplitudes[j] >= 0.0f) {
	    float d = coords[i].distance(centroids[j]);
	    //cout << "distance from point " << i << " to centroid " << j
	    // << " is " << d << endl;
	    if (d < mindistance) {

	      if (closest != -1 && centroids[j].distance(centroids[closest]) < 2) {
		switch (hack) {
		case 0 :
		  mindistance = d;
		  closest = j;
		  break;
		case 1 :
		  if (bins[j].size() > bins[closest].size()) {
		    mindistance = d;
		    closest = j;
		  }
		  break;
		case 2 :
		  if (age[j] > age[coords]) { //if (age[j] > age[i]) {
		    mindistance = d;
		    closest = j;
		  }
		  break;
		}
	      } else {
		mindistance = d;
		closest = j;
	      }
	    } 
	  }
	  }
	bins[closest].push_back(i);
      }
      
      // calculate new centroids / amplitudes
      
      go_again = false;
      for (i = 0; i < k; ++i) {
	float X = 0.0f; 
	float Y = 0.0f;
	amplitudes[i] = 0.0f;
	for (list<int>::iterator index = bins[i].begin(); 
	     index != bins[i].end(); ++index) {
	  amplitudes[i] += weights[*index];
	  X += coords[*index].x * weights[*index];
	  Y += coords[*index].y * weights[*index];
	}
	if (amplitudes[i] > 0.0f) {
	  centroids[i].x = X / amplitudes[i];
	  centroids[i].y = Y / amplitudes[i];
	  //amplitudes[i] /= float(bins[i].size());
	  if (centroids[i] != old_centroids[i]) {
	    go_again = true;
	  }
	} else {
	  amplitudes[i] = -1.0f;
	}
      }
      --maxruns;
    } while (go_again && maxruns);
    xused = 0;
    for (i = 0; i < k; ++i) {
      if (amplitudes[i] >= 0.0f) {
	++xused;
	++age[i];
      }
    }
    while (xused > n) {
      float minamp = 10000000.0;
      int minindex = -1;
      for (i = 0; i < k; ++i) {
	if (amplitudes[i] >= 0.0f && amplitudes[i] < minamp) {
	  minamp = amplitudes[i];
	  minindex = i;
	}
      }
      amplitudes[minindex] = -1.0f;
      age[i] = 0;
      --xused;
    }
  }


  vector<Point> centroids;
  vector<float> amplitudes;
  vector<int> age;
  vector<vector<float> > coordmap;
  vector<vector<int> > centroidmap;
  int k;
  int maxx,maxy;
  int xused;

private:


};

/*************************/

static int kmeans_start (ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[]);
static int kmeans_cmd (ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[]);


extern "C" int __export Kmeans_Init (Tcl_Interp *interp);

extern "C" int __export Kmeans_Init (Tcl_Interp *interp) {

  Tcl_CreateObjCommand (interp,"kmeans",kmeans_start,NULL,NULL);
  return TCL_OK;
}

/*******************************/

static int kmeans_start (ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[]) {

  if (objc < 2) {
    Tcl_AppendResult (interp,"usage: kmeans name k ",NULL);
    return TCL_ERROR;
  }

  char *name = Tcl_GetStringFromObj (objv[1],NULL);
  int k;
  
  Tcl_GetIntFromObj(interp,objv[2],&k);
  if (k <= 0) {
    Tcl_AppendResult (interp,"kmeans requires a positive integer",NULL);
    return TCL_ERROR;
  }

  Tcl_CreateObjCommand (interp,name,kmeans_cmd,
			ClientData(new KMeans(k)),NULL);
  return TCL_OK;
}

static int kmeans_cmd (ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj * const objv[]) {

  if (objc < 2) {
    Tcl_AppendResult (interp,"Requires \"init\" or a list of coordinates and weights.",NULL);
    return TCL_ERROR;
  }

  char *initcmd = Tcl_GetStringFromObj (objv[1],NULL);

  if (!strcmp(initcmd,"init")) {
    KMeans *kmeans = (KMeans *)(cd);
    kmeans->Init();
    return TCL_OK;
  } else if (!strcmp(initcmd,"debug")) {
    KMeans *kmeans = (KMeans *)(cd);
    Tcl_SetObjResult(interp,Tcl_NewIntObj(kmeans->xused));
    return TCL_OK;
  }

  if (objc < 3) {
    Tcl_AppendResult (interp,"Please supply a list of (x,y) coordinates and weights.",NULL);
    return TCL_ERROR;
  }

  Tcl_Obj **coordsl, **weightsl;
  int    numcoords, numweights;

  Tcl_ListObjGetElements(interp,objv[1],&numcoords,&coordsl);
  Tcl_ListObjGetElements(interp,objv[2],&numweights,&weightsl);

  KMeans *kmeans = (KMeans *)(cd);
  int numcentroids = kmeans->k;
  int hack = 0;

  if (objc > 3) {
    Tcl_GetIntFromObj(interp,objv[3],&numcentroids);
  }
  if (objc > 4) {
    Tcl_GetIntFromObj(interp,objv[4],&hack);
  }

  
  if (numcoords != numweights) {
    Tcl_AppendResult(interp,"I need the same number of coordinates and weights!",NULL);
    return TCL_ERROR;
  }

  vector<Point> coords(numcoords);
  vector<float> weights(numweights);

  //cout << numcoords << ' ' << numweights << endl;

  for (int i = 0; i < numcoords; ++i) {
    double d;
    Tcl_Obj *elem;
    Tcl_ListObjIndex(interp,coordsl[i],0,&elem);
    Tcl_GetDoubleFromObj(interp,elem,&d);
    //cout << d << ' ';
    coords[i].x = d;
    Tcl_ListObjIndex(interp,coordsl[i],1,&elem);
    Tcl_GetDoubleFromObj(interp,elem,&d);
    //cout << d << ' ';
    coords[i].y = d;
    Tcl_GetDoubleFromObj(interp,weightsl[i],&d);
    weights[i] = d;
    //cout << d << ' ';
    //cout << endl;
  }

  kmeans->Run(coords,weights,numcentroids,hack);
  
  Tcl_Obj *result = Tcl_NewListObj(0,NULL);
  for (i = 0; i < kmeans->k; ++i) {
    Tcl_Obj *entry = Tcl_NewListObj(0,NULL);
    Tcl_ListObjAppendElement(interp,entry,
			     Tcl_NewDoubleObj(kmeans->centroids[i].x));
    Tcl_ListObjAppendElement(interp,entry,
			     Tcl_NewDoubleObj(kmeans->centroids[i].y));
    Tcl_ListObjAppendElement(interp,entry,
			     Tcl_NewDoubleObj(kmeans->amplitudes[i]));
    Tcl_ListObjAppendElement(interp,result,entry);
  }

  Tcl_SetObjResult(interp,result);

  return TCL_OK;
}
