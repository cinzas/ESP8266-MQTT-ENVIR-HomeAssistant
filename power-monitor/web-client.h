//String inString = String(""); // storage for last page access

void SetDateTime(time_t t)
{
    int y = year(t)-2000;
    if (y < 0) sprintf(fdata,"Invalid");
    else      sprintf(fdata, "%02d.%02d.%02d %02d:%02d:%02d", day(t),month(t),y,hour(t),minute(t),second(t));
}

void showStatus(EthernetClient client)
{
  client.println(F("<h2>Current Cost Data</h2>"));

  client.print(F("<table><tr><th>Channel</th><th>Watts</th></tr>"));
  for (int i9 = 0; i9 < 10; i9++) {
    if (valueCount[i9] > 0) {
      client.print(F("<tr><td>"));
      client.print(i9);

      if (i9 == 0) {
        client.print(F(" (Consumption)"));
      } else if (i9 == PVOUTPUT_SOLARCHANNEL) {
        client.print(F(" (Generation)"));
      }

      client.print(F("</td><td>"));
      client.print((valueSum[i9] / valueCount[i9]));
      client.print(F("w ("));
      client.print(valueCount[i9]);
      client.print(F(")</td></tr>"));
    }
  }
  client.println(F("</table>"));

  client.println(F("<h2>Dates</h2>"));
  
  client.print(F("<table><tr><td>Startup</td><td>"));
  SetDateTime(dateStarted);
  client.print(fdata);
  client.print(F("</td></tr><tr><td>Now</td><td>"));
  SetDateTime(now());
  client.print(fdata);
  client.print(F("</td></tr><tr><td>Last failed submit</td><td>"));
  SetDateTime(dateFailed);
  client.print(fdata);
  client.print(F(" ("));
  client.print(failed_connections);
  client.println(F(")</td></tr></table>"));

  client.print(F("<p><small>"));
  client.print(VERSION);
  client.println(F("</small></p>"));
}

void ServeWebClients() 
{
    EthernetClient client = server.available();
    if (client) 
    {
      //inString = client.readStringUntil('\n');
      client.readStringUntil('\n');

      client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Content-Type: text/html"));
      client.println(F("Connection: close"));  // the connection will be closed after completion of the response
      client.println(F("Refresh: 6"));  // refresh the page automatically every 6 sec
      client.println();
      client.println(F("<!DOCTYPE HTML>"));
      client.println(F("<html>"));
      showStatus(client);
      client.println(F("</html>"));
      
      client.stop();
    }
}
