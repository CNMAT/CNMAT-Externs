/*
  
KMeans stuff by Amar Chaudhary 
 (translated into C for max tactex object)


*/


typedef struct taxpoint {
	float x;
	float y;
} Taxpoint;
  
float kmeans_taxeldistance (Taxpoint *p1, Taxpoint *p2) {
    float dx = p1->x - p2->x;
    float dy = p1->y - p2->y;
    //return dx * dx + dy * dy;
    return fabs(dx) + fabs(dy);
}


void kmeans_init(Tactex *x) 
{
	int i;
	int k = x->x_ncentroids;
	int maxx = x->x_ncols;
	int maxy = x->x_nrows;

	for (i = 0; i < k; ++i) {
		x->x_amplitudes[i] = 0.0f;
		x->x_centroids[i].x = rand() % maxx;
		x->x_centroids[i].y = rand() % maxy;
		x->x_age[i] = 0;
	}
	x->x_xused = k;
}


void kmeans_run (Taxpoint *coords, int coordssize, float *weights, int n, int hack) 
{
	int i;
	int k = x->x_ncentroids;
	int xused = x->x_xused;
	int maxx = x->x_ncols;
	int maxy = x->x_nrows;
	
    Taxpoint old_centroids[MAXCENTROIDS];
    Boolean inPreviousFrame[MAXCENTROIDS];

	int bins[5][128];
	int binssize[5];
	Boolean go_again = false;
	int maxruns = k;
    
    
	for (i = 0; i < k; ++i) {
		if (x->x_amplitudes[i] < 0.0f) {
			if (xused < n) {
				//switch (rand() % 4) {
				//case 0 : centroids[i] = Point(0,rand() % maxy); break;
				//case 1 : centroids[i] = Point(maxx,rand() % maxy); break;
				//case 2 : centroids[i] = Point(rand() % maxx,0); break;
				//case 3 : centroids[i] = Point(rand() % maxx,maxy); break;
				//}
				//cout << "Init point " << i << ": " << centroids[i].x << ' ' 
				//     << centroids[i].y << endl;
				x->x_centroids[i].x = rand() % maxx;
				x->x_centroids[i].y = rand() % maxy;
				x->x_amplitudes[i] = 0.0f;
				x->x_age[i] = 0;
				++xused;
	  
			}
		} else {
			inPreviousFrame[i] = true;
		}
	}
	
	do {
		for (int i = 0; i < k; ++i) {
			old_centroids[i] = x->x_centroids[i];
		}
      
		for (i = 0; i < coordssize; ++i) {
			float mindistance = 100000.0f;
			int closest = -1;
			for (int j = 0; j < k; ++j) {
				if (x->x_amplitudes[j] >= 0.0f) {
					float d = kmeans_distance(coords[i], centroids[j]);
					//cout << "distance from point " << i << " to centroid " << j
					// << " is " << d << endl;
					if (d < mindistance) {
						if (closest != -1 && kmeans_distance(centroids[j], centroids[closest]) < 2) {
							switch (hack) {
								case 0 :
									mindistance = d;
									closest = j;
									break;
								case 1 :
									if (binssize[j] > binssize[closest]) {
										mindistance = d;
										closest = j;
									}
									break;
								case 2 :
									if (x->x_age[j] > x->x_age[i]) {
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
			// **************************** need to fix this and add it back in!!
			//bins[closest].push_back(i);
		}
	      
		// calculate new centroids / amplitudes
	      
		go_again = false;
		for (i = 0; i < k; ++i) {
			float X = 0.0f; 
			float Y = 0.0f;
			x->x_amplitudes[i] = 0.0f;
			for (index = 0; index < binssize; ++index) {
				x->x_amplitudes[i] += weights[*index];
				X += coords[*index].x * weights[*index];
				Y += coords[*index].y * weights[*index];
			}
			if (x->x_amplitudes[i] > 0.0f) {
				centroids[i].x = X / x->x_amplitudes[i];
				centroids[i].y = Y / x->x_amplitudes[i];
				//amplitudes[i] /= float(bins[i].size());
				if (centroids[i] != old_centroids[i]) {
					go_again = true;
				}
			} else {
				x->x_amplitudes[i] = -1.0f;
			}
		}
		--maxruns;
	} while (go_again && maxruns);
	
	
	xused = 0;
	for (i = 0; i < k; ++i) {
		if (x->x_amplitudes[i] >= 0.0f) {
			++xused;
			++x->x_age[i];
		}
	}
	while (xused > n) {
		float minamp = 10000000.0;
		int minindex = -1;
		for (i = 0; i < k; ++i) {
			if (x->x_amplitudes[i] >= 0.0f && x->x_amplitudes[i] < minamp) {
				minamp = x->x_amplitudes[i];
				minindex = i;
			}
		}
		x->x_amplitudes[minindex] = -1.0f;
		x->x_age[i] = 0;
		--xused;
	}
	
	// restore updated values to obj instance
	x->x_xused = xused;
}
  