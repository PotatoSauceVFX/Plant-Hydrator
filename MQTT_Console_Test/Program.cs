using System;
using System.Threading.Tasks;

namespace MQTT_Console_Test
{
    public class Program
    {

        public static MQTT_Service mqttService;

        public static void Main(string[] args)
        {
            MainAsync(args).GetAwaiter().GetResult();

            Console.WriteLine("Press enter to exit.");
            while(Console.ReadKey().Key != ConsoleKey.Enter)
            {

            }

            mqttService.StopMqttServer().Wait();
        }

        public static async Task MainAsync(string[] args)
        {
            mqttService = new MQTT_Service();

            await mqttService.StartMqttServer();
        }
    }
}
