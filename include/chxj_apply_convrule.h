/*
 * Copyright (C) 2005-2008 Atsushi Konno All rights reserved.
 * Copyright (C) 2005 QSDN,Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __CHXJ_APPLY_CONVRULE_H__
#define __CHXJ_APPLY_CONVRULE_H__

#include "mod_chxj.h"

extern chxjconvrule_entry* chxj_apply_convrule(request_rec *r, apr_array_header_t *convrules);

#endif
/*
 * vim:ts=2 et
 */
