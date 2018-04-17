import { Injectable } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import { of } from 'rxjs/observable/of';
import { NetworkConfig } from './models/networkConfig';

@Injectable()
export class NetworkService {

  constructor() { }

  networkConfig: NetworkConfig = {
    
  };

  getNetworkConfig(): Observable<NetworkConfig> {
    return of(this.networkConfig);
  }
}
