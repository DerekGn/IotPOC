import { Component, OnInit } from '@angular/core';
import { NetworkService } from '../network.service';
import { NetworkConfig } from '../models/networkConfig';

@Component({
  selector: 'app-network',
  templateUrl: './network.component.html',
  styleUrls: ['./network.component.css']
})
export class NetworkComponent implements OnInit {

  constructor(private networkService: NetworkService) { }

  ngOnInit() {
    this.getNetworkConfig();
  }

  networkConfig: NetworkConfig;

  getNetworkConfig(): void {
    this.networkService.getNetworkConfig()
        .subscribe(config => this.networkConfig = config);
  }
}
