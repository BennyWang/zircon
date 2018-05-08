// Copyright 2016 The Fuchsia Authors
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <err.h>
#include <inttypes.h>

#include <fbl/ref_ptr.h>
#include <object/channel_dispatcher.h>
#include <object/handle.h>
#include <object/process_dispatcher.h>
#include <object/resource_dispatcher.h>

#include "priv.h"

// Create a new resource, child of the provided resource.
// On success, a new resource is created and handle is returned.
//
// The range low:high is inclusive on both ends, high must be
// greater than or equal low.
//
// If the parent resource is of type ZX_RSRC_KIND_ROOT, then there
// are no further limitations on the child resource created.
//
// Otherwise the kind of the child resource must be the same as
// the parent and the range of the child resource must be within
// the range of the parent.
zx_status_t sys_resource_create(zx_handle_t handle, uint32_t kind,
                                uint64_t low, uint64_t high,
                                user_out_handle* resource_out) {
    auto up = ProcessDispatcher::GetCurrent();

    if (high < low) {
        return ZX_ERR_INVALID_ARGS;
    }

    // Obtain the parent Resource
    // WRITE access is required to create a child resource
    zx_status_t status;
    fbl::RefPtr<ResourceDispatcher> parent;
    status = up->GetDispatcherWithRights(handle, ZX_RIGHT_WRITE, &parent);
    if (status) {
        return status;
    }

    // Only holders of the root resource passed out by userboot are permitted
    // to create resources using the syscall.
    if (parent->get_kind() != ZX_RSRC_KIND_ROOT) {
        return ZX_ERR_ACCESS_DENIED;
    }

    // Create a new Resource
    zx_rights_t rights;
    fbl::RefPtr<ResourceDispatcher> child;
    status = ResourceDispatcher::Create(&child, &rights, kind, low, high);
    if (status != ZX_OK) {
        return status;
    }

    // Create a handle for the child
    return resource_out->make(fbl::move(child), rights);
}
