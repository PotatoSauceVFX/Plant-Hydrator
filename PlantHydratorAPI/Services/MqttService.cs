using Microsoft.Extensions.Logging;
using MQTTnet;
using MQTTnet.Protocol;
using MQTTnet.Server;
using System;
using Microsoft.Extensions.Configuration;
using System.IO;
using System.Reflection;
using System.Security.Cryptography.X509Certificates;
using System.Security.Authentication;
using System.Text;

namespace PlantHydratorAPI.Services
{
    public class MqttService
    {

        private IMqttServer mqttServer;
        private MqttServerOptionsBuilder optionsBuilder;


        private readonly ILogger<MqttService> _logger;
        private readonly IConfiguration _configuration;


        public MqttService(ILogger<MqttService> logger, IConfiguration configuration)
        {
            _logger = logger;
            _configuration = configuration;

            // SSL
            var currentPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

            // Forgive me, fore I have sinned with a password like that. Plz don't hack me.
            var certificate = new X509Certificate2(Path.Combine(currentPath, "public_privatekey.pfx"), "Password", X509KeyStorageFlags.Exportable);


            // Settings
            optionsBuilder = new MqttServerOptionsBuilder();

            // Tons of SSL config
            optionsBuilder.WithoutDefaultEndpoint();
            optionsBuilder.WithEncryptedEndpoint();
            optionsBuilder.WithEncryptedEndpointPort(4242); // It's on a HTTPS web server now :) Ima sneaky boi
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


            // Start a MQTT server.
            mqttServer = new MqttFactory().CreateMqttServer();
            mqttServer.StartAsync(optionsBuilder.Build()).Wait();


            mqttServer.UseClientConnectedHandler((a) =>
            {
                OnClientConnect(a);
            });

            mqttServer.UseClientDisconnectedHandler((a) =>
            {
                OnClientDisconnect(a);
            });


            _logger.LogInformation("Started MQTT Server");
        }


        // When a client connects
        private void OnClientConnect(MqttServerClientConnectedEventArgs evnt)
        {
            var clientId = evnt.ClientId;

            _logger.LogDebug($"-----------------{clientId} Connected-----------------");

        }

        // When a client disconnects
        private void OnClientDisconnect(MqttServerClientDisconnectedEventArgs evnt)
        {
            var clientId = evnt.ClientId;

            _logger.LogDebug($"-----------------{clientId} Disconnected-----------------");
        }

        // Validate connection
        private void ValidateConnections(MqttConnectionValidatorContext context)
        {

            if (!context.Username.Equals(context.ClientId))
            {
                _logger.LogDebug($"{context.ClientId} failed to authenticate with username \"{context.Username}\".");
                context.ReasonCode = MqttConnectReasonCode.BadUserNameOrPassword;
                return;
            }

            if (!context.Password.Equals("ThisIsTheMostSecurePasswordYouHaveEverSeen"))
            {
                _logger.LogDebug($"{context.ClientId} failed to authenticate with password \"{context.Password}\".");
                context.ReasonCode = MqttConnectReasonCode.BadUserNameOrPassword;
                return;
            }

            var clientId = context.ClientId;

            
            _logger.LogDebug($"-----------------{clientId} Authenticated Successfully-----------------");

            context.ReasonCode = MqttConnectReasonCode.Success;
        }

        private void ConsumeApplicationMessage(MqttApplicationMessageInterceptorContext context)
        {
            _logger.LogDebug($"[{context.ClientId}] {Encoding.UTF8.GetString(context.ApplicationMessage.Payload)}");

            var clientId = context.ClientId;

            var data = context.ApplicationMessage;

        }

        public IMqttServer GetMqttServer()
        {
            return mqttServer;
        }
    }
}
