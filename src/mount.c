/* 
 * Copyright (c) 2015-2017, Gregory M. Kurtzer. All rights reserved.
 * 
 * Copyright (c) 2016-2017, The Regents of the University of California,
 * through Lawrence Berkeley National Laboratory (subject to receipt of any
 * required approvals from the U.S. Dept. of Energy).  All rights reserved.
 * 
 * This software is licensed under a customized 3-clause BSD license.  Please
 * consult LICENSE file distributed with the sources of this project regarding
 * your rights to use or distribute this software.
 * 
 * NOTICE.  This Software was developed under funding from the U.S. Department of
 * Energy and the U.S. Government consequently retains certain rights. As such,
 * the U.S. Government has been granted for itself and others acting on its
 * behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software
 * to reproduce, distribute copies to the public, prepare derivative works, and
 * perform publicly and display publicly, and to permit other to do so. 
 * 
 */


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include "config.h"
#include "lib/image/image.h"
#include "util/file.h"
#include "util/util.h"
#include "lib/config_parser.h"
#include "lib/registry.h"
#include "lib/privilege.h"

#ifndef SYSCONFDIR
#error SYSCONFDIR not defined
#endif


int main(int argc, char **argv) {
    struct image_object image;

    singularity_config_init(joinpath(SYSCONFDIR, "/singularity/singularity.conf"));

    // Before we do anything, check privileges and drop permission
    singularity_priv_init();
    singularity_priv_drop();

    singularity_registry_init();

    // Setting the location of the container
    singularity_image_path(singularity_registry_get("CONTAINER"));
    
    // Obtain the image object
    image = singularity_image_init(singularity_registry_get("CONTAINER"));

    singularity_image_open(&image, O_RDONLY);
    singularity_image_bind(&image);
    singularity_image_mount(&image, singularity_registry_get("MOUNTPOINT"));

    printf("%s is mounted at: %s\n", image.name, singularity_registry_get("MOUNTPOINT"));

    return(0);
}
