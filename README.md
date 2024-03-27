# AI-Smart-Power-meter

### Developed an AI Driven smart power meter using ESP32 for secure data transmission and PZEM-004T module for accurate energy measurement , the meter’s PCB connections were designed on the Easy EDA software. The data is securely transferred using MQTT protocol to the influxdb cloud platform for data storage and Node-Red for user-friendly data visualization with Grafana. 

### The circuit connection is given below: 
![PZEM-004T-100A-wiring-Diagram](https://github.com/AlexBandlamudi/AI-Smart-Power-meter/assets/111354340/1f35afeb-4215-4619-8e2c-e9a90c2cec63)


### THe AI model is build based on Autoregressive Integrated Moving Average (ARIMA)  which is a widely used statistical method for time series forecasting. It is a class of models that explains a given time series based on its own past values, lagged forecast errors, and the trend component. ARIMA models are capable of capturing a suite of different standard temporal structures in time series data. THis AI model is deployed on the Heroku Platform.

### Once the ESP32 sends the data to the influx DB, after 15 days the influxdb sends this historical data to the AI model(Heroku), The AI will predict the future power consumption based on this Historical data.The Energy data(15 days of that month) is securely sent from the influxdb to the server, The ARIMA AI model in the server predicts the future energy consumption within the same month. This enables users to make informed decisions and adjust their consumption patterns for optimal energy management.

### Result of the AI model: 

![Screenshot (475)](https://github.com/AlexBandlamudi/AI-Smart-Power-meter/assets/111354340/8c3e9f8d-d647-491a-a43d-4931d5f21100)

