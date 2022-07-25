#!/usr/bin/env python3

import wiotp.sdk.application
import ibm_watson

#credentials for IoT platform
options = { 
    "identity": {
        "appId": "6feat5"			#just a random name
    },
    "auth": {
        "key": "a-6feat5-zeidxric0g",		#API-key created in the IoT platform
        "token": "t_ES@RM6Ml3HaLKb(y"		#Token for the IoT platform
    }
}

#Credentials for Cloudant database
serviceBinding = {
    "name": "Cloudant-5n", 
    #"serviceName": "Cloudant-Fischertechnik", 
    "description":"connection between IoT platform and cloudant database",
    "type": "cloudant", 
    "credentials": { 
        "host": "e21c54a4-4228-49d2-8f74-7e9422ae4f18-bluemix.cloudantnosqldb.appdomain.cloud", 
        "port": 443, 
        "username": "e21c54a4-4228-49d2-8f74-7e9422ae4f18-bluemix", 
        "password": "86fa5e7388e52dab849ef5517f1ee986b8e048ae8781e44ea303fb6861c13759"
    }
}

#
appClient = wiotp.sdk.application.ApplicationClient(config=options)
print(appClient)
print()

#create a Service Binding
service = appClient.serviceBindings.create(serviceBinding)
print(service)

#Iterate through all service bindings of the IoT Platform
for s in appClient.serviceBindings:
    print(s.name)
    print(" - " + s.description)
    print(" - " + s.id)
    print()    

# Set up the connector
connector = appClient.dsc.create(name="connector2", serviceId=s.id, timezone="CET", description="A test connector", enabled=True, type='cloudant')
print(connector)
print()

# Set up destinations
connector.destinations.create(name="events", bucketInterval="DAY")
#connector.destinations.create(name="state", bucketInterval="DAY")

# Set up rules
rule1 = connector.rules.createEventRule(name="allevents", destinationName="events", typeId="*", eventId="*", description="send events", enabled=True)
#rule2 = connector.rules.createStateRule(name="allstate", destinationName="state", logicalInterfaceId="*")
