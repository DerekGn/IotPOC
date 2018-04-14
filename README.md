# IotPOC

A POC/Mock for an IOT gateway based on FreeRTOS and AngularJs SPA client

## Iot

The iot console application is a modified version of the freertos V9.0.0 FreeRTOS_Plus_TCP_and_FAT

## Iot REST API

The IOT application exposes a simple REST API that is used by the SPA for the purpose of developing the UI while hardware is being developed

### /api/gateway

#### GET /api/gateway

Retrives the gateways configuration

```json
{
    "name" : "",
    "nickname" : ""
}
```

**name** is the name that will be used by the gateway when requesting an IP address via DNS

**nickname** is the name that will be used by the gateway when requesting an IP address via DNS

#### PATCH /api/gateway

Can be used to update either the **devicename** or **nickname**

```json
{
    "devicename" : "",
    "nickname" : ""
}
```

### /api/network

#### GET /api/network

Retrieves the devices network configuration

```json
{
    "ip": [192,168,1,128],
    "networkmask": [255,255,255,0],
    "gateway": [192,168,1,1],
    "dns": [192,168,1,1],
    "mac": [0,17,34,51,68,72]
}
```

**ip** is the IP address that was objained via DNS
**networkmask** is the name that will be used by the gateway when requesting an IP address via DNS
**gateway** is the gateway address
**dns** is the IP address of the DNS service
**mac** the mac address of the device

### /api/radio

#### GET /api/radio

Retrives the radio configuration

```json
{
  "radios":
  [
    {
      "enabled": true,
      "faulted": false,
      "type": 0,
      "settings" :
      {
      }
    },
    {
      "enabled": true,
      "faulted": false,
      "type": 1,
      "settings" :
      {
      }
    }
  ]
}
```

**enabled** indicates if the radio is enabled, i.e. powered on
**faulted** indicates if the radio has faulted, i.e. indicates a power fault
**type** indicates the type of radio
**settings** these are the settings for the radio and will be different dependant on the radio type

#### GET /api/radio/{0}

Retrives a specified radios configuration. {0} is the Id of the radio from 0 ~ 3

```json
{
  "enabled": true,
  "faulted": false,
  "type": 0,
  "settings": {}
}
```

#### GET /api/radio/{0}/settings

Retrives a specified radios settings. {0} is the Id of the radio from 0 ~ 3

The settings will be different which is determined by the radio type 

#### PATCH /api/radio/{0}

Updates the radios configuration.

```json
{
  "enabled": true/false,
  "faulted": true/false,
  "type" : 0
}
```

**enabled** enables or disables the radio
**faulted** setting to false clears the power fault and reenables
**type** sets the radios type, this action will disable and enable the radio

#### PATCH /api/radio/{0}/settings

Update the radios settings, specified by {0} in the route