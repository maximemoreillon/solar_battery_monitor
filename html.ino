// Head partias
// Style
const String styles = "<style>"
  ".main{text-align:center;}"
  ".status{"
  "width: 35vmin;"
  "height: 35vmin;"
  "border-radius: 50%;"
  "border: 10px solid #ce0000;"
  "padding: 5px;"
  "background: white;"
  "background-clip: content-box;"
  "transition: background-color 0.5s;"
  "}"
  "#graph_container{"
  "height:60vh;"
  "}"
  "</style>";

// Body partials
const String header = "<div class='header'>"
  + String(HOSTNAME) +
  "</div>";

const String nav = "<div class='nav'>"
  "<a href='/'>Status</a>"
  "<a href='/update_form'>Firmware update</a>"
  "</div>";
  
const String footer= "<div class='footer'>"
  "<img src='https://cdn.maximemoreillon.com/logo/logo.png'>"
  "  <div>"
  "    <div>Battery monitor v" + String(VERSION) + "</div>"
  "    <div>Maxime Moreillon</div>"
  "  </div>"
  "</div>";

// Update form
String update_form = "<h2>Firmware update</h2>"
  "<form method='POST' action='/update' enctype='multipart/form-data'>"
  "<input type='file' name='update'>"
  "<input type='submit' value='Update'>"
  "</form>";

// Combinations
String head ="<head>"
  "<title>" + String(HOSTNAME) +"</title>"
  + styles +
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<link rel='stylesheet' type='text/css' href='https://cdn.maximemoreillon.com/master_css/master.css'>"
  "</head>";

String pre_main = "<html>"
  + head + 
  "<body>"
  "<div class='wrapper'>"
  + header + nav +
  "<div class='main'>";
  
String post_main = "</div>"
  + footer +
  "</div>"
  "</body>"
  "</html>";

// Root
String root_main = ""
  "<div id='graph_container'>"
  "</div>"
  "<script src='https://cdn.maximemoreillon.com/graph/graph.js'></script>"
  "<script>"
  "var graph = new Graph('graph_container');"
  "graph.axes.y.min = 11;"
  "graph.axes.y.max = 14;"
  "var batter_voltage_array = [];"
  "for(var i = 0; i < 100; i++) {"
  "  batter_voltage_array.push(12);"
  "}"
  "graph.plot(batter_voltage_array);"
  "websock = new WebSocket('ws://' + window.location.hostname + ':81/');"
  "websock.onopen = function(evt) { console.log('websocket open'); };"
  "websock.onclose = function(evt) { console.log('websocket close'); };"
  "websock.onerror = function(evt) { console.log(evt); };"
  "websock.onmessage = function(evt) {"
  "  var payload = JSON.parse(evt.data);"
  "  batter_voltage_array.shift();"
  "  batter_voltage_array.push(payload.battery_voltage);"
  "  graph.plot(batter_voltage_array);"
  ""
  "};"
  "</script>";

// Not found
String not_found = "<h2>Not found</h2>";
