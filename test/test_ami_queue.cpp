// Copyright 2008, The TPIE development team
// 
// This file is part of TPIE.
// 
// TPIE is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
// 
// TPIE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with TPIE.  If not, see <http://www.gnu.org/licenses/>

#include <tpie/portability.h>


#include <tpie/progress_indicator_arrow.h>

#include "app_config.h"        
#include "parse_args.h"

// Get the AMI_stack definition.
#include <tpie/queue.h>

using namespace tpie;

struct options app_opts[] = {
    { 0, NULL, NULL, NULL, 0 }
};

void parse_app_opts(int /* idx */, char* /* opt_arg */) {
}

int main(int argc, char **argv)
{
    progress_indicator_arrow* pi = new progress_indicator_arrow("Title","Desc",0,100,1);

    parse_args(argc, argv, app_opts, parse_app_opts);

    if (verbose) {
	std::cout << "test_size = " << test_size << "." << std::endl;
	std::cout << "test_mm_size = " << static_cast<TPIE_OS_OUTPUT_SIZE_T>(test_mm_size) << "." << std::endl;
	std::cout << "random_seed = " << random_seed << "." << std::endl;
    } else {
	std::cout << test_size << ' ' << static_cast<TPIE_OS_OUTPUT_SIZE_T>(test_mm_size) << ' ' << random_seed;
    }
    
    // Set the amount of main memory:
    MM_manager.set_memory_limit(test_mm_size);

    ami::queue<TPIE_OS_OFFSET> queue;
    
    pi->set_percentage_range(0,test_size);
    pi->set_description("Enqueue");
    
    // Push values.
    TPIE_OS_OFFSET ii;
    for (ii = 0; ii < test_size; ii++) {
	pi->step_percentage();
	queue.enqueue(ii);
    }
    pi->done("Done");
    
    if (verbose) {
	std::cout << "Queue size = " << queue.size() << std::endl;
    }
    
    TPIE_OS_OFFSET *jj;
    TPIE_OS_OFFSET last;
    TPIE_OS_OFFSET read = 0;

    queue.dequeue(&jj);
    last = *jj;
    
    pi->set_description("Dequeue");
    pi->reset();
    read++;
    pi->step_percentage();

    while(!queue.is_empty()) {

	ami::err ae = queue.dequeue(&jj);
	if(ae != ami::NO_ERROR) {
	    std::cout << "Error from queue received" << std::endl;
	}
	read++;
	pi->step_percentage();
	if(*jj != ++last) {
	    std::cout << std::endl << "Error in output: " << *jj << "!=" << last  << std::endl;
	}
    }

    pi->done("Done");

    if(read != test_size) {
	std::cout << "Error: Wrong amount of elements read, got: " << read << " expected: "<<test_size << std::endl;
    }

    if(verbose) {
	std::cout << "Queue size = " << queue.size() << std::endl;
    }
    
    return 0;
}
