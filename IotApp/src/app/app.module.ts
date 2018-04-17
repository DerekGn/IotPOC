import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';

import { AppComponent } from './app.component';
import { GatewayComponent } from './gateway/gateway.component';
import { GatewayService } from './gateway.service';
import { AppRoutingModule } from './/app-routing.module';
import { NetworkComponent } from './network/network.component';


@NgModule({
  imports: [
    BrowserModule,
    FormsModule,
    AppRoutingModule
  ],
  declarations: [
    AppComponent,
    GatewayComponent,
    NetworkComponent
  ],
  providers: [GatewayService],
  bootstrap: [AppComponent]
})
export class AppModule { }
