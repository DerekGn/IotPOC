import { Component, OnInit } from '@angular/core';
import { GatewayService } from '../gateway.service';
import { GatewayConfig } from './gatewayConfig';

@Component({
  selector: 'app-gateway',
  templateUrl: './gateway.component.html',
  styleUrls: ['./gateway.component.css']
})
export class GatewayComponent implements OnInit {

  constructor(private gatewayService: GatewayService) { }

  ngOnInit() {
    this.getGatewayConfig();
  }

  gatewayConfig: GatewayConfig;

  getGatewayConfig(): void {
    this.gatewayService.getGatewayConfig()
        .subscribe(config => this.gatewayConfig = config);
  }
}
