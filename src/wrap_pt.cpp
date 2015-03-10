#include "wrap_pt.h"
#include "lithosphere.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <vector>

class platec_api_list_elem
{
  public:
	platec_api_list_elem(size_t _id, lithosphere* _data) :
		data(_data), id(_id) { }

	lithosphere* data;
	size_t id;
};

extern lithosphere* platec_api_get_lithosphere(size_t);

static std::vector<platec_api_list_elem> lithospheres;
static size_t last_id = 1;


void* platec_api_create(long seed, size_t width, size_t height, float sea_level,
                         size_t erosion_period, float folding_ratio,
                         size_t aggr_overlap_abs, float aggr_overlap_rel,
                         size_t cycle_count, size_t num_plates)
{
	lithosphere* litho = new lithosphere(seed, width, height, sea_level,
		erosion_period, folding_ratio, aggr_overlap_abs,
		aggr_overlap_rel, cycle_count);
	litho->createPlates(num_plates);

	platec_api_list_elem elem(++last_id, litho);
	lithospheres.push_back(elem);	
	
	return litho;
}

void platec_api_destroy(void* litho)
{	
	for (size_t i = 0; i < lithospheres.size(); ++i)
		if (lithospheres[i].data == litho) {
			lithospheres.erase(lithospheres.begin()+i);
			break;
		}
}

const size_t* platec_api_get_agemap(size_t id)
{
	lithosphere* litho = platec_api_get_lithosphere(id);
	if (!litho)
		return NULL;

	return litho->getAgemap();
}

float* platec_api_get_heightmap(void *pointer)
{
	lithosphere* litho = (lithosphere*)pointer;
	float *res = litho->getTopography();
	return res;
}

size_t* platec_api_get_platesmap(void *pointer)
{
	lithosphere* litho = (lithosphere*)pointer;
	size_t *res = litho->getPlatesMap();
	return res;
}

lithosphere* platec_api_get_lithosphere(size_t id)
{
	for (size_t i = 0; i < lithospheres.size(); ++i)
		if (lithospheres[i].id == id)
			return lithospheres[i].data;

	return NULL;
}

size_t platec_api_is_finished(void *pointer)
{
	lithosphere* litho = (lithosphere*)pointer;
	if (litho->isFinished()) {
		return 1;
	} else {
		return 0;
	}
}

void platec_api_step(void *pointer)
{	   
	lithosphere* litho = (lithosphere*)pointer;
	litho->update();
}
