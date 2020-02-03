using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using MQTTnet;
using MQTTnet.Server;
using Newtonsoft.Json;
using PlantHydratorAPI.Services;

namespace PlantHydratorAPI.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class CountController : ControllerBase
    {

        private readonly MqttService _mqttService;

        private static CountData CountData = new CountData();

        public CountController(MqttService mqttService)
        {
            _mqttService = mqttService;
        }

        [HttpGet]
        public IActionResult GetCurrentCount()
        {
            return Ok(JsonConvert.SerializeObject(CountData));
        }

        [HttpPost("increment")]
        public async Task<IActionResult> IncrementCount()
        {
            CountData.Increment();

            var countDataJson = JsonConvert.SerializeObject(CountData);

            var message = new MqttApplicationMessageBuilder()
                .WithTopic("Count")
                .WithPayload(countDataJson)
                .WithExactlyOnceQoS()
                .WithRetainFlag()
                .Build();

            await _mqttService.GetMqttServer().PublishAsync(message);

            return Ok(countDataJson);
        }

        [HttpPost("decrement")]
        public async Task<IActionResult> DecrementCount()
        {
            CountData.Decrement();

            var countDataJson = JsonConvert.SerializeObject(CountData);

            var message = new MqttApplicationMessageBuilder()
                .WithTopic("Count")
                .WithPayload(countDataJson)
                .WithExactlyOnceQoS()
                .WithRetainFlag()
                .Build();

            await _mqttService.GetMqttServer().PublishAsync(message);

            return Ok(countDataJson);
        }

    }

    public class CountData
    {
        // Only writable inside the class with the class methods
        public int Count { get; private set; }

        public DateTime LastModified { get; private set; }

        public void Increment()
        {
            this.Count++;
            LastModified = DateTime.Now;
        }

        public void Decrement()
        {
            Count--;
            LastModified = DateTime.Now;
        }
    }
}