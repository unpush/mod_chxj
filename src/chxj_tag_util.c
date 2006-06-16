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
#include "chxj_tag_util.h"

/**
 * The value of the VALUE attribute that the object tag node maintains is
 * acquired.
 *
 * @param doc  [i] The pointer to the Doc structure to be scanned is
 *                 specified.
 * @param node [i] The tag node to be scanned is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the VALUE attribute that the object tag node maintains
 *         is returned. NULL is returned when not found.
 */
char*
qs_get_value_attr(Doc* doc, Node* node, request_rec* r)
{
  Attr*        attr;

  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if ((*name == 'v' || *name == 'V') && strcasecmp(name, "value") == 0)
      /*----------------------------------------------------------------------*/
      /* The VALUE attribute was found.                                       */
      /*----------------------------------------------------------------------*/
      return apr_pstrdup(r->pool, value);
  }

  /*--------------------------------------------------------------------------*/
  /* not found                                                                */
  /*--------------------------------------------------------------------------*/
  return NULL;
}

/**
 * The value of the checked tag is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure to be scanned is
 *                 specified.
 * @param tag  [i] The tag node to be scanned is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the checked tag is returned. NULL is returned when
 *         not found.
 */
char*
qs_get_checked_attr(Doc* doc, Node* tag, request_rec* r)
{
  Attr*        attr;
  int          found_flag = 0;

  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,tag);
       attr != NULL;
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    if ((*name == 'c' || *name == 'C') && strcasecmp(name, "checked") == 0) {
      /*----------------------------------------------------------------------*/
      /* The VALUE attribute was found.                                       */
      /*----------------------------------------------------------------------*/
      found_flag = 1;
      break;
    }
  }
  if (! found_flag)
    /*------------------------------------------------------------------------*/
    /* not found                                                              */
    /*------------------------------------------------------------------------*/
    return NULL;

  return qs_get_value_attr(doc, tag, r);
}


/**
 * The value of the type attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure to be scanned is
 *                 specified.
 * @param tag  [i] The tag node to be scanned is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the type attribute is returned. NULL is returned when
 *         not found.
 */
char*
qs_get_type_attr(Doc* doc, Node* tag, request_rec* r)
{
  Attr*        attr;

  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,tag);
       attr != NULL;
       attr = qs_get_next_attr(doc,attr)) {

    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if ((*name == 't' || *name == 'T') && strcasecmp(name, "type") == 0) 
      /*----------------------------------------------------------------------*/
      /* The VALUE attribute was found.                                       */
      /*----------------------------------------------------------------------*/
      return apr_pstrdup(r->pool, value);
  }
  /*--------------------------------------------------------------------------*/
  /* not found                                                                */
  /*--------------------------------------------------------------------------*/
  return NULL;
}

/**
 * The character string area in 0 bytes is allocated.
 *
 * @param r    [i]   To use POOL, the pointer to request_rec is specified.
 * @return The allocated 0 byte character string is returned.
 */
char*
qs_alloc_zero_byte_string(request_rec* r)
{
  char* tgt = apr_palloc(r->pool, 1);
  tgt[0] = '\0';

  return tgt;
}

/**
 * A consecutive head and the last WHITESPACE are removed.
 *
 * @param r    [i]   To use POOL, the pointer to request_rec is specified.
 * @param s    [i]   The character string that should be removed is specified.
 * @return The character string that has been removed is returned.
 */
char*
qs_trim_string(request_rec* r, char* s)
{
  char* ss = apr_pstrdup(r->pool, s);
  int len = strlen(s);
  int ii;

  ii = 0;
  for (ii = 0;is_white_space(*ss) && ii < len; ss++, ii++);

  ii = strlen(ss);
  for(;is_white_space(ss[ii-1]) && ii > 0; ii--);

  ss[ii] = '\0';

  return ss;
}

/**
 * The value of child TEXT of tag that maintains the SELECTED attribute is 
 * returned. 
 *
 * @param Doc  [i] The pointer to the Doc structure to be scanned is 
 *                 specified. 
 * @param node [i] The tag node to be scanned is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @reutrn  The value of child TEXT of tag that maintains the SELECTED 
 *          attribute is returned. NULL is returned when not found. 
 */
char*
qs_get_selected_value_text(Doc *doc, Node* node, request_rec* r)
{
  Node* child;
  Node* selchild;
  char* result   = NULL;

  for (child = qs_get_child_node(doc,node);
       child != NULL; 
       child = qs_get_next_node(doc,child)) {

    char* name = qs_get_node_name(doc,child);

    /*------------------------------------------------------------------------*/
    /* <OPTION> tag                                                           */
    /*------------------------------------------------------------------------*/
    if ((*name == 'o' || *name == 'O') && strcasecmp(name, "option") == 0) {
      Attr* attr;

      for (attr =  qs_get_attr(doc,child); 
           attr != NULL; 
           attr = qs_get_next_attr(doc,attr)) {

        char* name  = qs_get_attr_name(doc,attr);

        DBG1(r, "qs_get_selected_value name::[%s]" , name);

        if ((*name == 's'|| *name == 'S') && strcasecmp(name, "selected") == 0) {
          /*------------------------------------------------------------------*/
          /* SELECTED Value Found                                             */
          /*------------------------------------------------------------------*/
          selchild = qs_get_child_node(doc, child);
          if (! selchild) {
            DBG(r,"found selected tag but null node" );
            return NULL;
          }
          return qs_get_node_value(doc, selchild);
        }
      }
    }

    if ((result = qs_get_selected_value_text(doc, child, r)) != NULL)
      return result;
  }

  /*--------------------------------------------------------------------------*/
  /* not found                                                                */
  /*--------------------------------------------------------------------------*/
  return NULL;
}

/**
 * The value of tag that maintains the SELECTED attribute is acquired. 
 *
 * @param doc    [i]  The pointer to the Doc structure to be scanned is 
 *                    specified. 
 * @param node   [i]  The SELECT tag node is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of tag that maintains the SELECTED attribute is 
 *         returned. NULL is returned when not found. 
 */
char*
qs_get_selected_value(Doc* doc, Node* node, request_rec* r)
{
  Node*         child;
  char*         result    = NULL;

  for (child = qs_get_child_node(doc,node); 
       child != NULL; 
       child = qs_get_next_node(doc,child)) {

    char* name = qs_get_node_name(doc,child);

    /*------------------------------------------------------------------------*/
    /* <OPTION> tag                                                           */
    /*------------------------------------------------------------------------*/
    if ((*name == 'o' || *name == 'O') && strcasecmp(name, "option") == 0) {
      Attr* attr;
      for (attr = qs_get_attr(doc,child); 
           attr != NULL; 
           attr = qs_get_next_attr(doc,attr)) 
      {
        char* name  = qs_get_attr_name(doc,attr);

        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                        "qs_get_selected_value name::[%s]" , name);
        if (strcasecmp(name, "selected") == 0) 
        {
          /*------------------------------------------------------------------*/
          /* SELECTED Value Found                                             */
          /*------------------------------------------------------------------*/
          return qs_get_value_attr(doc, child, r);
        }
      }
    }
    result = qs_get_selected_value(doc, child, r);
    if (result != NULL)
    {
      return result;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* not found                                                                */
  /*--------------------------------------------------------------------------*/
  return NULL;
}

/**
 * The value of the SIZE attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the SIZE attribute
 *                 is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the SIZE attribute is returned. NULL is
 *         returned when not is.
 */
char*
qs_get_name_attr(Doc* doc, Node* tag, request_rec* r)
{
  Attr* attr;

  for (attr = qs_get_attr(doc,tag); attr != NULL; attr = qs_get_next_attr(doc,attr))
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "name") == 0)
    {
      return apr_pstrdup(r->pool, value);
    }
  }

  return NULL;
}

/**
 * The value of the SIZE attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the SIZE attribute
 *                 is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the SIZE attribute is returned. NULL is
 *         returned when not is.
 */
char*
qs_get_size_attr(Doc* doc, Node* tag, request_rec* r)
{
  Attr* attr;

  for (attr = qs_get_attr(doc,tag); attr != NULL; attr = qs_get_next_attr(doc,attr))
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "size") == 0)
    {
      return apr_pstrdup(r->pool, value);
    }
  }

  return NULL;
}

/**
 * The value of the ACCESSKEY attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the ACCESSKEY attribute
 *                 is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the ACCESSKEY attribute is returned. NULL is
 *         returned when not is.
 */
char*
qs_get_accesskey_attr(Doc* doc, Node* tag, request_rec* r)
{
  Attr* attr;

  for (attr = qs_get_attr(doc,tag); attr != NULL; attr = qs_get_next_attr(doc,attr))
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "accesskey") == 0)
    {
      return apr_pstrdup(r->pool, value);
    }
  }

  return NULL;
}

/**
 * The value of the ISTYLE attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the ISTYLE attribute
 *                 is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the ISTYLE attribute is returned. NULL is
 *         returned when not is.
 */
char*
qs_get_istyle_attr(Doc* doc, Node* tag, request_rec* r)
{
  Attr*        attr;

  for (attr = qs_get_attr(doc,tag); 
       attr != NULL; 
       attr = qs_get_next_attr(doc,attr))
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "istyle") == 0)
    {
      return apr_pstrdup(r->pool, value);
    }
  }

  return NULL;
}

/**
 * The value of the MAXLENGTH attribute is acquired from the tag node of the
 * object.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the MAXLENGTH attribute
 *                 is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the MAXLENGTH attribute is returned. NULL is
 *         returned when not is.
 */
char*
qs_get_maxlength_attr(Doc* doc, Node* tag, request_rec* r)
{
  Attr*        attr;

  for (attr = qs_get_attr(doc,tag);
       attr != NULL; 
       attr = qs_get_next_attr(doc,attr))
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "maxlength") == 0)
    {
      return apr_pstrdup(r->pool, value);
    }
  }
  return NULL;
}

/**
 * It is scanned whether the CHECKBOX tag of the object is CHECKED. 
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the CHECKBOX attribute
 *                 is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return 1 is returned when it is CHECKED and, additionally, 0 is returned. 
 */
int
qs_is_checked_checkbox_attr(Doc* doc, Node* tag, request_rec* r)
{
  Attr* attr;

  for (attr = qs_get_attr(doc,tag);
       attr != NULL; 
       attr = qs_get_next_attr(doc,attr))
  {
    char* name  = qs_get_attr_name(doc,attr);

    if (strcasecmp(name, "checked") == 0)
    {
      return 1;
    }
  }
  return 0;
}


int
chxj_chxjif_is_mine(device_table_t* spec, Doc* doc, Node* tag)
{
  request_rec* r = doc->r;
  Attr* attr;

  for (attr = qs_get_attr(doc,tag);
       attr != NULL; 
       attr = qs_get_next_attr(doc,attr))
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "lang") == 0)
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "lang found [%s] spec [%d]", value, spec->html_spec_type);
      if (strcasecmp(value, "xhtml") == 0) 
      {
        if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0)
        {
          /* Yes , it is mine */
          return 1;
        }
      }
      else
      if (strcasecmp(value, "hdml") == 0)
      {
        if (spec->html_spec_type == CHXJ_SPEC_Hdml)
        {
          /* Yes , it is mine */
          return 1;
        }
      }
      else
      if (strcasecmp(value, "jhtml") == 0)
      {
        if (spec->html_spec_type == CHXJ_SPEC_Jhtml)
        {
          /* Yes , it is mine */
          return 1;
        }
      }
      else
      if (strcasecmp(value, "chtml") == 0)
      {
        switch (spec->html_spec_type) {
        case CHXJ_SPEC_Chtml_1_0:
        case CHXJ_SPEC_Chtml_2_0:
        case CHXJ_SPEC_Chtml_3_0:
        case CHXJ_SPEC_Chtml_4_0:
        case CHXJ_SPEC_Chtml_5_0:
          return 1;
        default:
          break;
        }
      }
    }
  }

  /* No, it is not mine. */
  return 0;
}

/**
 * The value of the DESTLANG attribute is acquired from the tag node of the
 * object.
 *
 * @param doc  [i] The pointer to the Doc structure at the output
 *                 destination is specified.
 * @param tag  [i] The tag node to want to acquire the DESTLANG attribute
 *                 is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the DESTLANG attribute is returned. NULL is
 *         returned when not is.
 */
char*
qs_get_destlang_attr(Doc* doc, Node* tag, request_rec* r)
{
  Attr*        attr;

  for (attr = qs_get_attr(doc,tag);
       attr != NULL; 
       attr = qs_get_next_attr(doc,attr))
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "destlang") == 0)
    {
      return apr_pstrdup(r->pool, value);
    }
  }
  return NULL;
}
/**
 * The value of the PARSE attribute is acquired.
 *
 * @param doc  [i] The pointer to the Doc structure to be scanned is
 *                 specified.
 * @param tag  [i] The tag node to be scanned is specified.
 * @param r    [i] To use POOL, the pointer to request_rec is specified.
 * @return The value of the PARSE attribute is returned. NULL is returned when
 *         not found.
 */
char*
qs_get_parse_attr(Doc* doc, Node* tag, request_rec* r)
{
  Attr*        attr;

  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,tag);
       attr != NULL;
       attr = qs_get_next_attr(doc,attr))
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "parse") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* The VALUE attribute was found.                                       */
      /*----------------------------------------------------------------------*/
      return apr_pstrdup(r->pool, value);
    }
  }
  /*--------------------------------------------------------------------------*/
  /* not found                                                                */
  /*--------------------------------------------------------------------------*/
  return NULL;
}

/*
 * vim:ts=2 et
 */
