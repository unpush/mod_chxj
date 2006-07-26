/*
 * Copyright (C) 2005 QSDN,Inc. All rights reserved.
 * Copyright (C) 2005 Atsushi Konno All rights reserved.
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
#ifndef __QS_PARSE_TAG_H__
#define __QS_PARSE_TAG_H__

#include "qs_parse_string.h"

extern Node* qs_parse_tag(
  Doc*         doc, 
  const char*  s, 
  int          len) ;

extern Node* qs_new_tag(
  Doc*         doc);

extern Node* qs_add_attr(
  Doc*         doc,
  Node*        node, 
  Attr*        attr);


#endif
/*
 * vim:ts=2 et
 */

