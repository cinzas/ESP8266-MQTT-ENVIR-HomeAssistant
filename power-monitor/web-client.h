
void SetDateTime(time_t t)
{
    int y = year(t)-2000;
    if (y < 0) sprintf(fdata, "Invalid");
    else      sprintf(fdata, "%02d.%02d.%02d %02d:%02d:%02d", day(t),month(t),y,hour(t),minute(t),second(t));
}

void ServeWebClients() 
{
  String inString = F("<head><title>EnergyProxy</title>");
  inString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  inString += F("<link rel='stylesheet' href='http://code.jquery.com/mobile/1.3.1/jquery.mobile-1.3.1.min.css' type='text/css'>");
  inString += F("<script src='http://code.jquery.com/jquery-1.9.1.min.js' type='text/javascript'></script>");
  inString += F("<script src='http://code.jquery.com/mobile/1.3.1/jquery.mobile-1.3.1.min.js' type='text/javascript'></script>");
  inString += F("</head>");
  inString += F("<body>");

  inString += F("<div data-role='page' id='page_opts'>");
  inString += F("<div data-role='header'><h3>EnergyProxy</h3></div>");
  inString += F("<div data-role='content'>");

  inString += F("<fieldset data-role='controlgroup' data-type='horizontal'>");
  inString += F("<input type='radio' name='opt_group' id='basic' onclick='toggle_opt()' checked><label for='basic'>Info</label>");
  inString += F("<input type='radio' name='opt_group' id='other' onclick='toggle_opt()'><label for='other'>Settings</label>");
  inString += F("</fieldset>");
  
  
  inString += F("<div id='div_basic'>");
  inString += F("<table>");
  inString += F("<tr><th>Channel</th><th>Watts</th></tr>");
  for (int i9 = 0; i9 < 10; i9++) {
    if (valueCount[i9] > 0) {
      inString += F("<tr><td>");
      inString += i9;

      if (i9 == 0) {
        inString += F("<small>(Consumption)</small>");
      } else if (i9 == PVOUTPUT_SOLARCHANNEL) {
        inString += F("<small>(Generation)</small>");
      }

      inString += F("</td><td>");
      inString += (valueSum[i9] / valueCount[i9]);
      inString += F("w (");
      inString += valueCount[i9];
      inString += F(")</td></tr>");
    }
  }
  
  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");
  inString += F("<tr><td><b>Startup:</b></td><td>");
  SetDateTime(dateStarted);
  inString += fdata;
  inString += F("</td><td></td></tr>");
  inString += F("<tr><td><b>Failed&nbsp;submission:</b></td><td>");
  SetDateTime(dateFailed);
  inString += fdata;
  if (failed_connections > 0) {
    inString += F("<span style='color:red;'>(");
    inString += failed_connections;
    inString += F(")</span>");
  }  
  inString += F("</td><td></td></tr>");
  inString += F("<tr><td><b>Now:</b></td><td>");
  SetDateTime(now());
  inString += fdata;
  inString += F("</td></tr>");
  inString += F("</table><br />");
  inString += F("</div>");

  
  inString += F("<div id='div_other' style='display:none;'>");
  inString += F("<table cellpadding='2'>");
  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");
  
  inString += F("<tr><td><b>WIFI SSID:</b></td><td><label id='lbl_rssi'>");
  inString += WIFI_SSID;
  inString += F("</label></td></tr>");

  inString += F("<tr><td><b>WiFi&nbsp;Signal:</b></td><td><label id='lbl_rssi'>");
  long rssi = WiFi.RSSI();
  if (rssi > -71) {
    inString += F("Good");
  } else if (rssi > -81) {
    inString += F("Weak");    
  } else {
    inString += F("Poor");    
  }
  inString += F(" (");
  inString += rssi;
  inString += F(" dbBm)");
  inString += F("</label></td></tr>");
  
  inString += F("<tr><td><b>UTC Offset Hours:</b></td><td><label id='lbl_rssi'>");
  inString += UTC_OFFSET_HOURS;
  inString += F("</label></td></tr>");
  
  inString += F("<tr><td><b>Update Interval:</b></td><td><label id='lbl_rssi'>");
  inString += UPDATE_INTERVAL;
  inString += F("</label></td></tr>");
    
  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>"); 
  inString += F("<tr><td colspan='2'><input type='checkbox' id='cb1' checked MQTT_ENABLE disabled><label for='cb1'>MQTT Enabled</label></td></tr>");
  inString += F("<tr><td><b>Server:</b></td><td><label id='lbl_rssi'>");
  inString += MQTT_SERVER;
  inString += F("</label></td></tr>");
  inString += F("<tr><td><b>Topic:</b></td><td><label id='lbl_rssi'>");
  inString += MQTT_TOPIC;
  inString += F("</label></td></tr>");

  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");
  inString += F("<tr><td colspan='2'><input type='checkbox' id='cb2' ");
  #if PVOUTPUT_ENABLE == 1
    inString += F("checked");
  #endif
  inString += F(" disabled><label for='cb2'>PVOutput Enabled</label></td></tr>");
  inString += F("<tr><td><b>System Id:</b></td><td><label id='lbl_rssi'>");
  inString += PVOUTPUT_SYSTEMID;
  inString += F("</label></td></tr>");

  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");
  inString += F("<tr><td colspan='2'><input type='checkbox' id='cb3' ");
  #if THINGSPEAK_ENABLE == 1
    inString += F("checked");
  #endif
  inString += F(" disabled><label for='cb3'>ThingSpeak Enabled</label></td></tr>");
  inString += F("<tr><td><b>Key:</b></td><td><label id='lbl_rssi'>");
  inString += THINGSPEAK_KEY;
  inString += F("</label></td></tr>");

  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");
  inString += F("<tr><td colspan='2'><input type='checkbox' id='cb4' ");
  #if XIVELY_ENABLE == 1
    inString += F("checked");
  #endif
  inString += F(" disabled><label for='cb4'>Xively Enabled</label></td></tr>");
  inString += F("<tr><td><b>Feed Id:</b></td><td><label id='lbl_rssi'>");
  inString += XIVELY_FEEDID;
  inString += F("</label></td></tr>");
  
  inString += F("</table>");
  inString += F("</div>");

  
  inString += F("<div data-role='footer' data-theme='c'><h5>");
  inString += VERSION;
  inString += F("</h5></div>");
  inString += F("</div>");

  inString += F("<script>");
  inString += F("function eval_cb(n)  {return $(n).is(':checked')?1:0;}");
  inString += F("function toggle_opt() {");
  inString += F("$('#div_basic').hide();");
  inString += F("$('#div_other').hide();");
  inString += F("if(eval_cb('#basic')) $('#div_basic').show();");
  inString += F("if(eval_cb('#other')) $('#div_other').show();");
  inString += F("};");
  inString += F("</script>");
  inString += F("</body>");

  server.send(200, "text/html", inString);
}
