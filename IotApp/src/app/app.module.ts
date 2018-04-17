import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';

import { AppComponent } from './app.component';
import { GatewayComponent } from './gateway/gateway.component';
import { GatewayService } from './services/gateway.service';
import { AppRoutingModule } from './/app-routing.module';
import { NetworkComponent } from './network/network.component';
import { RadioComponent } from './radio/radio.component';
import { NetworkService } from './services/network.service';
import { RadioService } from './services/radio.service';


@NgModule({
  imports: [
    BrowserModule,
    FormsModule,
    AppRoutingModule,
    HttpClientModule
  ],
  declarations: [
    AppComponent,
    GatewayComponent,
    NetworkComponent,
    RadioComponent
  ],
  providers: [
    GatewayService,
    NetworkService,
    RadioService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
