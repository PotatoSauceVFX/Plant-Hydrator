using MQTTnet;
using MQTTnet.Protocol;
using MQTTnet.Server;
using System;
using System.IO;
using System.Reflection;
using System.Security.Authentication;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace MQTT_Console_Test
{
    public class MQTT_Service
    {

        public IMqttServer mqttServer;
        private MqttServerOptionsBuilder optionsBuilder;

        public MQTT_Service(int port = 443)
        {
            // SSL
            var currentPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            var certificate = new X509Certificate2(Path.Combine(currentPath, "public_privatekey.pfx"), "Password", X509KeyStorageFlags.Exportable);


            // Settings
            optionsBuilder = new MqttServerOptionsBuilder();

            // Tons of SSL config
            optionsBuilder.WithoutDefaultEndpoint();
            optionsBuilder.WithEncryptedEndpoint();
            optionsBuilder.WithEncryptedEndpointPort(port);
            optionsBuilder.WithEncryptionCertificate(certificate.Export(X509ContentType.Pfx));
            optionsBuilder.WithEncryptionSslProtocol(SslProtocols.Tls12);


            optionsBuilder.WithDefaultCommunicationTimeout(TimeSpan.FromSeconds(5));    // 5 sec timeout
            optionsBuilder.WithApplicationMessageInterceptor((context) =>
            {
                this.ConsumeApplicationMessage(context);
            });

            optionsBuilder.WithConnectionValidator((context) =>
            {
                this.ValidateConnections(context);
            });

            
            mqttServer = new MqttFactory().CreateMqttServer();


            mqttServer.UseClientConnectedHandler((a) =>
            {
                OnClientConnect(a);
            });

            mqttServer.UseClientDisconnectedHandler((a) =>
            {
                OnClientDisconnect(a);
            });

        }

        private void ValidateConnections(MqttConnectionValidatorContext context)
        {
            if (context.ClientId.Equals("Server", StringComparison.OrdinalIgnoreCase))
            {
                context.ReasonCode = MqttConnectReasonCode.ClientIdentifierNotValid;

                Console.WriteLine($"{context.ClientId} failed to authenticate with username \"{context.Username}\".");

                return;
            }

            if (context.Username != "SudoPluto")
            {
                context.ReasonCode = MqttConnectReasonCode.BadUserNameOrPassword;

                Console.WriteLine($"{context.ClientId} failed to authenticate with username \"{context.Username}\".");

                return;
            }

            if (context.Password != "sdfjdoiusfe")
            {
                context.ReasonCode = MqttConnectReasonCode.BadUserNameOrPassword;

                Console.WriteLine($"{context.ClientId} failed to authenticate with password \"{context.Password}\".");

                return;
            }

            context.ReasonCode = MqttConnectReasonCode.Success;
        }

        private void ConsumeApplicationMessage(MqttApplicationMessageInterceptorContext context)
        {
            var data = context.ApplicationMessage;
            var payloadString = Encoding.UTF8.GetString(context.ApplicationMessage.Payload);

            Console.WriteLine(
                $"{context.ClientId}: {data.Topic} {payloadString}"
            );
        }

        private void OnClientConnect(MqttServerClientConnectedEventArgs evnt)
        {
            Console.WriteLine($"Client {evnt.ClientId} Connected!");
        }

        private void OnClientDisconnect(MqttServerClientDisconnectedEventArgs evnt)
        {
            Console.WriteLine($"Client {evnt.ClientId} Disconnected!");
        }

        public async Task StartMqttServer()
        {
            // Start a MQTT server.
            await mqttServer.StartAsync(optionsBuilder.Build());
            Console.WriteLine("Started MQTT Server!");
        }

        public async Task StopMqttServer()
        {
            // Stop a MQTT server.
            await mqttServer.StopAsync();
        }

    }
}
