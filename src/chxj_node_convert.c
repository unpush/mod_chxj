/*
 * Copyright (C) 2005-2008 QSDN,Inc. All rights reserved.
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
#include "mod_chxj.h"

tag_handlers chxj_tag_handlers[] = {
  {
    .type    = CHXJ_SPEC_UNKNOWN,
    .handler = NULL,
  },
  {
    .type    = CHXJ_SPEC_Chtml_1_0,
    .handler = chtml10_handler,
  },
  {
    .type    = CHXJ_SPEC_Chtml_2_0,
    .handler = chtml20_handler,
  },
  {
    .type    = CHXJ_SPEC_Chtml_3_0,
    .handler = chtml30_handler,
  },
  {
    .type    = CHXJ_SPEC_Chtml_4_0,
    .handler = chtml30_handler,
  },
  {
    .type    = CHXJ_SPEC_Chtml_5_0,
    .handler = chtml30_handler,
  },
  {
    .type    = CHXJ_SPEC_Chtml_6_0,
    .handler = chtml30_handler,
  },
  {
    .type    = CHXJ_SPEC_Chtml_7_0,
    .handler = chtml30_handler,
  },
  {
    .type    = CHXJ_SPEC_XHtml_Mobile_1_0,
    .handler = xhtml_handler,
  },
  {
    .type    = CHXJ_SPEC_Hdml,
    .handler = hdml_handler,
  },
  {
    .type    = CHXJ_SPEC_Jhtml,
    .handler = jhtml_handler,
  },
  {
    .type    = CHXJ_SPEC_HTML,
    .handler = NULL,
  },
};


/**
 * It is main processing of conversion from CHTML to XXML. 
 *
 * @param spec    [i]   
 * @param r       [i]   
 * @param pdoc    [i/o] The pointer to the XXML structure is specified. 
 * @param doc     [i/o] The pointer to the XXML structure is specified. 
 * @param node    [i]   The pointer to a current node is specified. 
 * @param indent  [i]   The depth of the node processing it now is specified. 
 *
 * @return The character string after it converts it is returned. 
 */
char *
chxj_node_convert(
  device_table *spec,
  request_rec  *r,
  void         *pdoc, 
  Doc          *doc, 
  Node         *node, 
  int          indent
)
{
  Node         *child;
  tag_handler  *handlers;


  handlers = chxj_tag_handlers[spec->html_spec_type].handler;

  /*--------------------------------------------------------------------------*/
  /* It is the main loop of the conversion processing.                        */
  /*--------------------------------------------------------------------------*/
  for (child = qs_get_child_node(doc,node);
       child;
       child = qs_get_next_node(doc,child)) {

    char *name;

    name = qs_get_node_name(doc,child);

    switch(*name) {
    case 'h':
    case 'H':
      /*----------------------------------------------------------------------*/
      /* <HTML>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "html") == 0) {
        if (handlers[tagHTML].start_tag_handler) 
          handlers[tagHTML].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagHTML].end_tag_handler)
          handlers[tagHTML].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <HEAD>                                                               */
      /*----------------------------------------------------------------------*/
      else 
      if (strcasecmp(name, "head") == 0) {
        if (handlers[tagHEAD].start_tag_handler) 
          handlers[tagHEAD].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagHEAD].end_tag_handler)
          handlers[tagHEAD].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <HR>                                                                 */
      /*----------------------------------------------------------------------*/
      else 
      if (strcasecmp(name, "hr") == 0) {
        if (handlers[tagHR].start_tag_handler) 
          handlers[tagHR].start_tag_handler(pdoc, child);

        if (handlers[tagHR].end_tag_handler)
          handlers[tagHR].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <H1>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "h1") == 0) {
        if (handlers[tagH1].start_tag_handler) 
          handlers[tagH1].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagH1].end_tag_handler)
          handlers[tagH1].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <H2>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "h2") == 0) {
        if (handlers[tagH2].start_tag_handler) 
          handlers[tagH2].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagH2].end_tag_handler)
          handlers[tagH2].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <H3>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "h3") == 0) {
        if (handlers[tagH3].start_tag_handler) 
          handlers[tagH3].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagH3].end_tag_handler)
          handlers[tagH3].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <H4>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "h4") == 0) {
        if (handlers[tagH4].start_tag_handler) 
          handlers[tagH4].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagH4].end_tag_handler)
          handlers[tagH4].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <H5>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "h5") == 0) {
        if (handlers[tagH5].start_tag_handler) 
          handlers[tagH5].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagH5].end_tag_handler)
          handlers[tagH5].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <H6>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "h6") == 0) {
        if (handlers[tagH6].start_tag_handler) 
          handlers[tagH6].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagH6].end_tag_handler)
          handlers[tagH6].end_tag_handler(pdoc, child);
      }
      break;


    case 'p':
    case 'P':
      /*----------------------------------------------------------------------*/
      /* <P>                                                                  */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "p") == 0) {
        if (handlers[tagP].start_tag_handler) 
          handlers[tagP].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagP].end_tag_handler)
          handlers[tagP].end_tag_handler(pdoc, child);
      }
      else
      /*----------------------------------------------------------------------*/
      /* <PRE>                                                                */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "pre") == 0) {
        if (handlers[tagPRE].start_tag_handler) 
          handlers[tagPRE].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagPRE].end_tag_handler)
          handlers[tagPRE].end_tag_handler(pdoc, child);
      }
      break;

    case 'n':
    case 'N':
      /*----------------------------------------------------------------------*/
      /* <NOBR>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "nobr") == 0) {
        if (handlers[tagNOBR].start_tag_handler) 
          handlers[tagNOBR].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagNOBR].end_tag_handler)
          handlers[tagNOBR].end_tag_handler(pdoc, child);
      }
      break;

    case 'u':
    case 'U':
      /*----------------------------------------------------------------------*/
      /* <UL>                                                                 */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "ul") == 0) {
        if (handlers[tagUL].start_tag_handler) 
          handlers[tagUL].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagUL].end_tag_handler)
          handlers[tagUL].end_tag_handler(pdoc, child);
      }
      break;

    case 'l':
    case 'L':
      /*----------------------------------------------------------------------*/
      /* <LI>                                                                 */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "li") == 0) {
        if (handlers[tagLI].start_tag_handler) 
          handlers[tagLI].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagLI].end_tag_handler)
          handlers[tagLI].end_tag_handler(pdoc, child);
      }
      else
      /*----------------------------------------------------------------------*/
      /* <LEGEND>                                                             */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "legend") == 0) {
        if (handlers[tagLEGEND].start_tag_handler) 
          handlers[tagLEGEND].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagLEGEND].end_tag_handler)
          handlers[tagLEGEND].end_tag_handler(pdoc, child);
      }
      else
      /*----------------------------------------------------------------------*/
      /* <LABEL>                                                              */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "label") == 0) {
        if (handlers[tagLABEL].start_tag_handler) 
          handlers[tagLABEL].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagLABEL].end_tag_handler)
          handlers[tagLABEL].end_tag_handler(pdoc, child);
      }
      
      break;

    case 'o':
    case 'O':
      /*----------------------------------------------------------------------*/
      /* <OL>                                                                 */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "ol") == 0) {
        if (handlers[tagOL].start_tag_handler) 
          handlers[tagOL].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagOL].end_tag_handler)
          handlers[tagOL].end_tag_handler(pdoc, child);
      }
      else
      /*----------------------------------------------------------------------*/
      /* <OPTION>                                                             */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "option") == 0) {
        if (handlers[tagOPTION].start_tag_handler) 
          handlers[tagOPTION].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagOPTION].end_tag_handler)
          handlers[tagOPTION].end_tag_handler(pdoc, child);
      }

      break;

    case 'm':
    case 'M':
      /*----------------------------------------------------------------------*/
      /* <META>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "meta") == 0) {
        if (handlers[tagMETA].start_tag_handler) 
          handlers[tagMETA].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagMETA].end_tag_handler)
          handlers[tagMETA].end_tag_handler(pdoc, child);
      }
      break;

    case 'b':
    case 'B':
      /*----------------------------------------------------------------------*/
      /* <B>                                                                  */
      /*----------------------------------------------------------------------*/
      if (strlen(name) == 1) {
        if (handlers[tagB].start_tag_handler) 
          handlers[tagB].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagB].end_tag_handler)
          handlers[tagB].end_tag_handler(pdoc, child);
      }
      else
      /*----------------------------------------------------------------------*/
      /* <BASE>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "base") == 0) {
        if (handlers[tagBASE].start_tag_handler) 
          handlers[tagBASE].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagBASE].end_tag_handler)
          handlers[tagBASE].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <BODY>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "body") == 0) {
        if (handlers[tagBODY].start_tag_handler) 
          handlers[tagBODY].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagBODY].end_tag_handler)
          handlers[tagBODY].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <BR>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "br") == 0) {
        if (handlers[tagBR].start_tag_handler) 
          handlers[tagBR].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagBR].end_tag_handler)
          handlers[tagBR].end_tag_handler(pdoc, child);
      }
      break;

    case 'a':
    case 'A':
      /*----------------------------------------------------------------------*/
      /* <A>                                                                  */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "a") == 0) {
        if (handlers[tagA].start_tag_handler) 
          handlers[tagA].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagA].end_tag_handler)
          handlers[tagA].end_tag_handler(pdoc, child);
      }
      break;

    case 'f':
    case 'F':
      /*----------------------------------------------------------------------*/
      /* <FONT>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "font") == 0) {
        if (handlers[tagFONT].start_tag_handler) 
          handlers[tagFONT].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagFONT].end_tag_handler)
          handlers[tagFONT].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <FORM>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "form") == 0) {
        if (handlers[tagFORM].start_tag_handler) 
          handlers[tagFORM].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagFORM].end_tag_handler)
          handlers[tagFORM].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <FIELDSET>                                                           */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "fieldset") == 0) {
        if (handlers[tagFIELDSET].start_tag_handler) 
          handlers[tagFIELDSET].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagFIELDSET].end_tag_handler)
          handlers[tagFIELDSET].end_tag_handler(pdoc, child);
      }
      break;

    case 'i':
    case 'I':
      /*----------------------------------------------------------------------*/
      /* <INPUT>                                                              */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "input") == 0) {
        if (handlers[tagINPUT].start_tag_handler) 
          handlers[tagINPUT].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagINPUT].end_tag_handler)
          handlers[tagINPUT].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <IMG>                                                                */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "img") == 0) {
        if (handlers[tagIMG].start_tag_handler) 
          handlers[tagIMG].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagIMG].end_tag_handler)
          handlers[tagIMG].end_tag_handler(pdoc, child);
      }
      break;

    case 's':
    case 'S':
      /*----------------------------------------------------------------------*/
      /* <SELECT>                                                             */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "select") == 0) {
        if (handlers[tagSELECT].start_tag_handler) 
          handlers[tagSELECT].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagSELECT].end_tag_handler)
          handlers[tagSELECT].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <STYLE>                                                              */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "style") == 0) {
        if (handlers[tagSTYLE].start_tag_handler) 
          handlers[tagSTYLE].start_tag_handler(pdoc, child);
        if (handlers[tagSTYLE].end_tag_handler)
          handlers[tagSTYLE].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <SPAN>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "span") == 0) {
        if (handlers[tagSPAN].start_tag_handler) 
          handlers[tagSPAN].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagSPAN].end_tag_handler)
          handlers[tagSPAN].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <SMALL>                                                              */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "small") == 0) {
        if (handlers[tagSMALL].start_tag_handler) 
          handlers[tagSMALL].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagSMALL].end_tag_handler)
          handlers[tagSMALL].end_tag_handler(pdoc, child);
      }
      break;
  
    case 'd':
    case 'D':
      /*----------------------------------------------------------------------*/
      /* <DIV>                                                                */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "div") == 0) {
        if (handlers[tagDIV].start_tag_handler) 
          handlers[tagDIV].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagDIV].end_tag_handler)
          handlers[tagDIV].end_tag_handler(pdoc, child);
      }
      break;

    case 'c':
    case 'C':
      /*----------------------------------------------------------------------*/
      /* <CENTER>                                                             */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "center") == 0) {
        if (handlers[tagCENTER].start_tag_handler) 
          handlers[tagCENTER].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagCENTER].end_tag_handler)
          handlers[tagCENTER].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <CHXJ:IF>                                                            */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "chxj:if") == 0) {
        if (chxj_chxjif_is_mine(spec, doc, child)) {
          char* parse_attr;

          parse_attr = qs_get_parse_attr(doc, child, r);

          if (parse_attr && strcasecmp(parse_attr, "true") == 0) {
            chxj_node_convert(spec, r, pdoc, doc, child, indent+1);
          }
          else {
            if (handlers[tagCHXJIF].start_tag_handler)
              handlers[tagCHXJIF].start_tag_handler(pdoc, child);
          }
        }
      }
      break;

    case 't':
    case 'T':
      /*----------------------------------------------------------------------*/
      /* <TEXTAREA>                                                           */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "textarea") == 0) {
        if (handlers[tagTEXTAREA].start_tag_handler) 
          handlers[tagTEXTAREA].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagTEXTAREA].end_tag_handler)
          handlers[tagTEXTAREA].end_tag_handler(pdoc, child);
      }
      else
      /*----------------------------------------------------------------------*/
      /* <TITLE>                                                              */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "title") == 0) {
        if (handlers[tagTITLE].start_tag_handler) 
          handlers[tagTITLE].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagTITLE].end_tag_handler)
          handlers[tagTITLE].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <TABLE>                                                              */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "table") == 0) {
        if (handlers[tagTABLE].start_tag_handler) 
          handlers[tagTABLE].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagTABLE].end_tag_handler)
          handlers[tagTABLE].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <TBODY>                                                              */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "tbody") == 0) {
        if (handlers[tagTBODY].start_tag_handler) 
          handlers[tagTBODY].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagTBODY].end_tag_handler)
          handlers[tagTBODY].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <TH>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "th") == 0) {
        if (handlers[tagTH].start_tag_handler) 
          handlers[tagTH].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagTH].end_tag_handler)
          handlers[tagTH].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <TR>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "tr") == 0) {
        if (handlers[tagTR].start_tag_handler) 
          handlers[tagTR].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagTR].end_tag_handler)
          handlers[tagTR].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* <TD>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "td") == 0) {
        if (handlers[tagTD].start_tag_handler) 
          handlers[tagTD].start_tag_handler(pdoc, child);

        chxj_node_convert(spec, r, pdoc, doc, child, indent+1);

        if (handlers[tagTD].end_tag_handler)
          handlers[tagTD].end_tag_handler(pdoc, child);
      }
      /*----------------------------------------------------------------------*/
      /* NORMAL TEXT                                                          */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "text") == 0) {
        if (handlers[tagTEXT].start_tag_handler)
          handlers[tagTEXT].start_tag_handler(pdoc, child);
      }
      break;

    default:
      chxj_node_convert(spec, r, pdoc, doc, child, indent+1);
    }
  }

  return NULL;
}
/*
 * vim:ts=2 et
 */
