#include "rtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mach/mach_time.h>
#include <time.h>
#include <float.h>

int main(int argc, char **argv){
	long l = 10;
	if(argc > 1){
		char *end = NULL;
		l = strtol(argv[1], &end, 0);
	}
	srand(mach_absolute_time());
	t_rtree *tree = rtree_new();
	int i;

	FILE *td = fopen("/Users/john/Development/cnmat/trunk/max/externals/misc/maquette/rtree_test_data.txt", "w+");
	double insertion_time = 0;
	for(i = 0; i < l; i++){
		t_rtree_rect *rect = (t_rtree_rect *)malloc(sizeof(t_rtree_rect));
		rect->xmin = ((double)rand() / RAND_MAX) * 1000.;
		rect->xmax = rect->xmin + ((double)rand() / RAND_MAX) * 100.;
		rect->ymin = ((double)rand() / RAND_MAX) * 1000.;
		rect->ymax = rect->ymin + ((double)rand() / RAND_MAX) * 100.;

		//printf("%f %f %f %f\n", rect->xmin, rect->xmax, rect->ymin, rect->ymax);
		fprintf(td, "%f %f %f %f\n", rect->xmin, rect->xmax, rect->ymin, rect->ymax);
		clock_t start = clock();
		rtree_insert(tree, *rect, (void *)rect);
		clock_t end = clock();
		insertion_time += ((double)(end - start)) / CLOCKS_PER_SEC;
		/*
		char buf[128];
		sprintf(buf, "/Users/john/Development/cnmat/trunk/max/externals/misc/maquette/rtree_nodes_%d.txt", i + 1);
		FILE *nodes = fopen(buf, "w+");
		rtree_write(tree, nodes);
		fclose(nodes);
		*/
	}
	printf("avg insertion time = %f\n", insertion_time / l);
	//fclose(td);
	td = fopen("/Users/john/Development/cnmat/trunk/max/externals/misc/maquette/rtree_nodes.txt", "w+");
	rtree_write(tree, td);
	fclose(td);

	double avg_success_time = 0, avg_fail_time = 0;
	int nsuccess = 0, nfail = 0;
	for(i = 0; i < l; i++){
		clock_t start, end;
		t_rtree_rect rect;
		rect.xmin = rect.xmax = ((double)rand() / RAND_MAX) * 1000.;
		rect.ymin = rect.ymax = ((double)rand() / RAND_MAX) * 1000.;
		//printf("searching for %f %f\n", rect.xmin, rect.ymin);
		//fprintf(fp, "%f %f\n", rect.xmin, rect.ymin);
		start = clock();
		long argc = 0;
		void **argv = NULL;
		rtree_search(tree, rect, &argc, &argv);
		end = clock();
		int j;
		for(j = 0; j < argc; j++){
			printf("searching for %f %f\n", rect.xmin, rect.ymin);
			printf("argv[%d] = %p\n", j, argv[j]);
			t_rtree_rect *r = (t_rtree_rect *)(argv[j]);
			printf("found %f %f %f %f\n", r->xmin, r->xmax, r->ymin, r->ymax);
		}
		/*
		if(argc){
			avg_success_time += ((double)(end - start)) / CLOCKS_PER_SEC;
			nsuccess++;
			//printf("searching for %f %f\n", rect.xmin, rect.ymin);
			//printf("found %f <= %f <= %f, %f <= %f <= %f\n", n->rect.xmin, rect.xmin, n->rect.xmax, n->rect.ymin, rect.ymin, n->rect.ymax);
		}else{
			avg_fail_time += ((double)(end - start)) / CLOCKS_PER_SEC;
			nfail++;
			//printf("not found\n");
		}
		*/
	}
	//fclose(fp);

	printf("lookup time success = %f\n", avg_success_time / nsuccess);
	printf("lookup time fail = %f\n", avg_fail_time / nfail);
	//rtree_print(tree);
	//rtree_write(tree, "/Users/john/Development/cnmat/trunk/max/externals/misc/maquette/rtree_test.txt");

	return 0;
}
