/**
 * Load weather data from dafwa weather stations and send it to the pebble watchface.
 */

var xhrRequest = function(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function() {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  var api_key = 'F609B62D128B08EF40486B3E.apikey';
  console.log(pos.coords.latitude + ' ' + pos.coords.longitude);
  // Get the nearest weather station.
  var nearbyUrl = 'https://test-api.agric.wa.gov.au/v1/stations.json/nearby/' + pos.coords.latitude + '/' + pos.coords.longitude + '/100?api_key=' + api_key;
  xhrRequest(nearbyUrl, 'GET', function(responseText) {
    var json = JSON.parse(responseText);
    var r = json.result[0];
    var stationName = r.station_name;
    var stationCode = r.station_code;
    console.log(stationName + ' ' + stationCode);
    
    // Now load the weather data
    var weatherUrl = 'https://test-api.agric.wa.gov.au/v1/stations.json/'+ stationCode +'/latest?api_key=' + api_key;
    xhrRequest(weatherUrl, 'GET', function(responseText) {
      var json = JSON.parse(responseText);
      json = json.result[0];
      console.log(JSON.stringify(responseText));
      var stationCode = json.station[0].station_code;
      var stationName = json.station[0].station_name;
      var airTemp = json.air_temp[0].value;
      var feelsLike = json.feels_like[0].value;
      var humidity = json.humidity[0].value;
      var windDirection = json.wind_10min_avg[0].compass_direction;
      var windSpeed = json.wind_10min_avg[0].speed_average;
      var dewPoint = json.dewpoint[0].value;
      var soilTemp = json.soil_temp[0].soiltemp_degc_ave;
      var rainfall9am = json.rainfall_from_9am[0].value;

      var dictionary = {
        'KEY_STATIONNAME': stationName,
        'KEY_STATIONCODE': stationCode,
        'KEY_AIRTEMP': airTemp,
        'KEY_FEELSLIKE': feelsLike,
        'KEY_HUMIDITY': humidity,
        'KEY_WINDDIRECTION': windDirection,
        'KEY_WINDSPEED': windSpeed,
        'KEY_DEWPOINT': dewPoint,
        'KEY_SOILTEMP': soilTemp,
        'KEY_RAINFALL9AM': rainfall9am
      };

      Pebble.sendAppMessage(dictionary, function(e) {
        console.log('Weather info sent to Pebble successfully!');
      }, function(e) {
        console.log('Error sending weather info to Pebble!');
      }); 
    });
  });
}

function locationError(pos) {
  console.log('Error requesting location!');
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
  locationSuccess, locationError,
    { timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS ready!');
  getWeather();
});

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', function(e) {
  console.log('Appmessage received!');
  getWeather();
});
                     
