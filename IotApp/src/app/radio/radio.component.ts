import { Component, OnInit } from '@angular/core';
import { RadioService } from '../services/radio.service';
import { RadioConfig } from '../models/radioConfig';

@Component({
  selector: 'app-radio',
  templateUrl: './radio.component.html',
  styleUrls: ['./radio.component.css']
})
export class RadioComponent implements OnInit {

  selectedRadioConfig: RadioConfig;

  radioConfigs: RadioConfig[];

  constructor(private radioService: RadioService) { }

  ngOnInit() {
    this.getRadioConfig();
  }

  getRadioConfig(): void {
    this.radioService.getRadioConfig()
        .subscribe(configs => this.radioConfigs = configs);
  }

  onSelect(radioConfig: RadioConfig): void {
    this.selectedRadioConfig = radioConfig;
  }
}
