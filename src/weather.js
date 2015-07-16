/**
 * Load weather data from openweathermap.org and send it to the pebble watchface.
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
  var url = 'https://test-api.agric.wa.gov.au/v1/stations'
  
  
  var url = 'https://test-api.agric.wa.gov.au/v1/stations.json/nearby/' + pos.coords.latitude + '/' + pos.coords.longitude + '/100?api_key==' + api_key;

  xhrRequest(url, 'GET', function(responseText) {
    var json = JSON.parse(responseText);
    if (json.cod !== 200) {
      console.log(json.message);
    }
    else {
      console.log(JSON.stringify(responseText));
      var airtemp = json.air_temp[0].value;
      var feelslike = json.feels_like[0].value;
      var humidity = json.humidity[0].value;
      var winddirection = json.wind_10min_avg[0].value;
      var windspeed = json.wind_10min_avg[0].speed_average;
      var dewpoint = json.dewpoint[0].value;
      var soiltemp = json.soiltemp[0].soiltemp_degc_ave;
      var rainfall9am = json.rainfall_from_9am.value;
      
      var dictionary = {
        'KEY_AIRTEMP': airtemp,
        'KEY_FEELSLIKE': feelslike,
        'KEY_HUMIDITY': humidity,
        'KEY_WINDDIRECTION': winddirection,
        'KEY_WINDSPEED': windspeed,
        'KEY_DEWPOINT': dewpoint,
        'KEY_SOILTEMP': soiltemp,
        'KEY_RAINFALL9AM': rainfall9am
      };
      
      Pebble.sendAppMessage(dictionary, function(e) {
        console.log('Weather info sent to Pebble successfully!');
      }, function(e) {
        console.log('Error sending weather info to Pebble!');
      }
     ); 
    }
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
                     
