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
package com.qsdn.filter;


import java.io.IOException;

import java.util.Iterator;
import java.util.Map;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;

/**
 * CHXJFilter for mod_chxj
 *
 * converter for _chxj_c_ or _chxj_r_ prefix parameter.
 *
 * @author Atsushi Konno <konno@qsdn.co.jp>
 */
public class CHXJFilter
  implements Filter 
{
  private String enc;
  private boolean ignore = false;
  private boolean decodeHttpGetParameter = true;

  public void destroy() 
  {
  }

  public void doFilter(
    ServletRequest request,
    ServletResponse response,
    FilterChain chain)
    throws IOException, ServletException 
  {
    CHXJRequestWrapper chxjRequest = new CHXJRequestWrapper((HttpServletRequest)request);
    chain.doFilter(chxjRequest, response);
  }

  public void init(FilterConfig config)
    throws ServletException 
  {
    enc = config.getInitParameter("encoding");
  }
}
/*
 * vim:ts=2 et
 */
