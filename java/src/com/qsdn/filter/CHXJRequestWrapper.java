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

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Map;
import java.util.TreeMap;
import java.util.Iterator;
import java.util.Enumeration;
import java.util.StringTokenizer;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;

public class CHXJRequestWrapper 
  extends HttpServletRequestWrapper 
{
  private class ParamNameEnumeration 
    implements Enumeration 
  {
    private Iterator iterator;

    private ParamNameEnumeration(Map map) 
    {
      if (map == null)
      {
        iterator = new LinkedList().iterator();
      }
      else
      {
        iterator = map.keySet().iterator();
      }
    }

    public boolean hasMoreElements() 
    {
      return iterator.hasNext();
    }

    public Object nextElement() 
    {
      return iterator.next();
    }
  }


  public CHXJRequestWrapper(HttpServletRequest request) 
  {
    super(request);
  }

  public String getQueryString() 
  {
    String queryString = super.getQueryString();
    if (queryString == null)
    {
      return null;
    }
    StringBuffer newQueryString = new StringBuffer();

    StringTokenizer st = new StringTokenizer(queryString, "&");

    for(int ii=0; st.hasMoreTokens(); ii++) 
    {
      String token = st.nextToken();
      int p = token.indexOf("=");
      if (p == -1)
      {
        continue;
      }

      String key = token.substring(0, p);
      String val = token.substring(p + 1);
      if (key.startsWith("_chxj_c_")
      ||  key.startsWith("_chxj_r_")) 
      {
        key = key.substring(8);
        if (key.length() == 0 ||  val.length() == 0)
        {
          continue;
        }
      }
      if (ii != 0) 
      {
        newQueryString.append("&");
      }
      newQueryString.append(key);
      newQueryString.append("=");
      try {
        newQueryString.append(URLDecoder.decode(val, "Windows-31J"));
      }
      catch (java.io.UnsupportedEncodingException ee)
      {
        throw new RuntimeException(ee);
      }
    }

    String resultQueryString = newQueryString.toString();
    if (resultQueryString.length() == 0)
    {
       return null;
    }
    return new String(resultQueryString);
  }

  public String getParameter(String name) 
  {
    Map pmap = getParameterMap();
    if (pmap == null)
    {
      return null;
    }

    String[] vals = (String[])pmap.get(name);
    if (vals == null)
    {
      return null;
    }

    return vals[0];
  }

  public Map getParameterMap() 
  {
    Map pmap = super.getParameterMap();
    if (pmap == null)
    {
      return null;
    }

    Map newpmap = new TreeMap();

    if ("get".equalsIgnoreCase(super.getMethod())) 
    {
      String queryString = this.getQueryString();
      if (queryString != null) 
      {
        StringTokenizer st = new StringTokenizer(queryString, "&");
        while (st.hasMoreTokens()) 
        {
          String token = st.nextToken();
          int pos = token.indexOf("=");
          if (pos == -1)
          {
            continue;
          }
          String key = token.substring(0, pos);
          String val = token.substring(pos+1);
          if (val != null) 
          {
            ArrayList list = (ArrayList)newpmap.get(key);
            if (list == null) 
            {
              list = new ArrayList();
              newpmap.put(key, list);
            }
            list.add(val);
          }
        }
      }
      Iterator it = newpmap.keySet().iterator();
      while (it.hasNext()) 
      {
        String key = (String)it.next();
        ArrayList list = (ArrayList)newpmap.get(key);
        String[] newvals = new String[list.size()];
        for (int ii = 0; ii < list.size(); ii++)
        {
          try {
            newvals[ii] = URLDecoder.decode((String)list.get(ii), "Windows-31J");
          }
          catch (java.io.UnsupportedEncodingException ee)
          {
            throw new RuntimeException(ee);
          }
        }
        newpmap.put(key, newvals);
      }
    }
    else 
    {
      Iterator it = pmap.keySet().iterator();
      while (it.hasNext()) 
      {
        String key = (String)it.next();
        if (key.startsWith("_chxj_dmy"))
        {
          continue;
        }

        if (key.startsWith("_chxj_c_")
        ||  key.startsWith("_chxj_r_"))
        {
          if (key.substring(8).length() == 0)
          {
            continue;
          }
          String[] vals = (String[])pmap.get(key);
          ArrayList list = new ArrayList();
          for (int ii = 0; ii < vals.length; ii++)
          {
            if (vals[ii].length() > 0)
            {
              list.add(vals[ii]);
            }
          }
          if (list.size() == 0)
          {
            continue;
          }
          String[] newvals = new String[list.size()];
          for (int ii = 0; ii < list.size(); ii++)
          {
            try {
              newvals[ii] = URLDecoder.decode((String)list.get(ii), "Windows-31J");
            }
            catch (java.io.UnsupportedEncodingException ee)
            {
              throw new RuntimeException(ee);
            }
          }
          newpmap.put(key.substring(8), newvals);
        }
        else
        {
          String[] vals = (String[])pmap.get(key);
          ArrayList list = new ArrayList();
          for (int ii = 0; ii < vals.length; ii++)
          {
            if (vals[ii].length() > 0)
            {
              list.add(vals[ii]);
            }
          }
          if (list.size() == 0)
          {
            continue;
          }
          String[] newvals = new String[list.size()];
          for (int ii = 0; ii < list.size(); ii++)
          {
            try {
              newvals[ii] = URLDecoder.decode((String)list.get(ii), "Windows-31J");
            }
            catch (java.io.UnsupportedEncodingException ee)
            {
              throw new RuntimeException(ee);
            }
          }
          newpmap.put(key, newvals);
        }
      }
    }

    return newpmap;
  }

  public Enumeration getParameterNames() 
  {
    Map map = getParameterMap();
    return new ParamNameEnumeration(map);
  }

  public String[] getParameterValues(String name) 
  {
    if (name == null)
    {
      throw new NullPointerException();
    }

    Map pmap = getParameterMap();
    if (pmap == null)
    {
      return null;
    }

    return (String[])pmap.get(name);
  }
}
/*
 * vim:ts=2 et
 */
