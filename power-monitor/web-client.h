
void SetDateTime(time_t t)
{
    int y = year(t)-2000;
    if (y < 0) sprintf(fdata, "Invalid");
    else      sprintf(fdata, "%02d.%02d.%02d %02d:%02d:%02d", day(t),month(t),y,hour(t),minute(t),second(t));
}

//void showStatus()
//{
//  client[1].println(F("<h2>Current Cost Data</h2>"));
//
//  client[1].print(F("<table><tr><th>Channel</th><th>Watts</th></tr>"));
//  for (int i9 = 0; i9 < 10; i9++) {
//    if (valueCount[i9] > 0) {
//      client[1].print(F("<tr><td>"));
//      client[1].print(i9);
//
//      if (i9 == 0) {
//        client[1].print(F(" <small>(Consumption)</small>"));
//      } else if (i9 == PVOUTPUT_SOLARCHANNEL) {
//        client[1].print(F(" <small>(Generation)</small>"));
//      }
//
//      client[1].print(F("</td><td>"));
//      client[1].print((valueSum[i9] / valueCount[i9]));
//      client[1].print(F("w ("));
//      client[1].print(valueCount[i9]);
//      client[1].print(F(")</td></tr>"));
//    }
//  }
//  client[1].println(F("</table>"));
//
//  client[1].println(F("<h3>Dates</h3>"));
//  
//  client[1].print(F("<table><tr><td>Startup</td><td>"));
//  SetDateTime(dateStarted);
//  client[1].print(fdata);
//  client[1].print(F("</td></tr><tr><td>Failed submission</td><td>"));
//  SetDateTime(dateFailed);
//  client[1].print(fdata);
//  if (failed_connections > 0) {
//    client[1].print(F(" <span style='color:red;'>("));
//    client[1].print(failed_connections);
//    client[1].println(F(")</span>"));
//  }  
//  client[1].print(F("</td></tr><tr><td>Now</td><td>"));
//  SetDateTime(now());
//  client[1].print(fdata);
//  client[1].println(F("</td></tr></table>"));
//
//  client[1].print(F("<p><small>"));
//  client[1].print(VERSION);
//  client[1].println(F("</small></p>"));
//}

void ServeWebClients() 
{
  String inString = F("<!DOCTYPE HTML><html>");
  inString += F("<h2>Current Cost Data</h2>");
  inString += F("<table><tr><th>Channel</th><th>Watts</th></tr>");
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
  inString += F("</table>");

  inString += F("<h3>Dates</h3>");
  inString += F("<table><tr><td>Startup</td><td>");
  SetDateTime(dateStarted);
  inString += fdata;
  inString += F("</td></tr><tr><td>Failed submission</td><td>");
  SetDateTime(dateFailed);
  inString += fdata;
  if (failed_connections > 0) {
    inString += F("<span style='color:red;'>(");
    inString += failed_connections;
    inString += F(")</span>");
  }  
  inString += F("</td></tr><tr><td>Now</td><td>");
  SetDateTime(now());
  inString += fdata;
  inString += F("</td></tr></table>");

  inString += F("<p><small>");
  inString += VERSION;
  inString += F("</small></p>");
  inString += F("</html>");

  server.send(200, "text/html", inString);
  
    //client[1] = server.available();
    //if (client[1]) {
    //  //inString = client[1].readStringUntil('\n');
    //  client[1].readStringUntil('\n');

    //  client[1].println(F("HTTP/1.1 200 OK"));
    //  client[1].println(F("Content-Type: text/html"));
    //  client[1].println(F("Connection: close"));  // the connection will be closed after completion of the response
    //  client[1].println(F("Refresh: 6"));  // refresh the page automatically every 6 sec
    //  client[1].println();
    //  client[1].println(F("<!DOCTYPE HTML>"));
    //  client[1].println(F("<html>"));
    //  showStatus();
    //  client[1].println(F("</html>"));
    //  
    //  client[1].stop();
    //}
}
