import { Injectable } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import { of } from 'rxjs/observable/of';
import { GatewayConfig } from './gateway/gatewayConfig';

@Injectable()
export class GatewayService {

  constructor() { }

  gatewayConfig: GatewayConfig = {
    name: 'name',
    nickname: 'nickname'
  };

  getGatewayConfig(): Observable<GatewayConfig> {
    return of(this.gatewayConfig);
  }
}
