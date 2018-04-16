import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';

import { AppComponent } from './app.component';
import { GatewayComponent } from './gateway/gateway.component';
import { GatewayService } from './gateway.service';


@NgModule({
  declarations: [
    AppComponent,
    GatewayComponent
  ],
  imports: [
    BrowserModule,
    FormsModule
  ],
  providers: [GatewayService],
  bootstrap: [AppComponent]
})
export class AppModule { }
