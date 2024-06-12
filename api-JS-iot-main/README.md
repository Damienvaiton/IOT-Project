# API Wallwatcher - Radar de recul
Cette API permet de relier une projet Firebase à l' ESP32 Cam et l'application mobile afin de communiquer facilement est via un noeud central

## Version: 1.0.0

### /register-esp

#### POST
##### Description:



##### Parameters

| Name | Located in | Description | Required | Schema |
| ---- | ---------- | ----------- | -------- | ---- |
| id | query |  | No | string |

##### Responses

| Code | Description |
| ---- | ----------- |
| 200 | OK |

### /set-distance

#### POST
##### Description:



##### Parameters

| Name | Located in | Description | Required | Schema |
| ---- | ---------- | ----------- | -------- | ---- |
| id | query |  | No | string |
| distance | query |  | No | string |

##### Responses

| Code | Description |
| ---- | ----------- |
| 200 | OK |

### /distance

#### GET
##### Description:



##### Responses

| Code | Description |
| ---- | ----------- |
| 200 | OK |

### /set-tone-frequency

#### POST
##### Description:



##### Parameters

| Name | Located in | Description | Required | Schema |
| ---- | ---------- | ----------- | -------- | ---- |
| id | query |  | No | string |
| tone | query |  | No | string |
| frequency | query |  | No | string |

##### Responses

| Code | Description |
| ---- | ----------- |
| 200 | OK |

### /note-frequency

#### GET
##### Description:



##### Responses

| Code | Description |
| ---- | ----------- |
| 200 | OK |
