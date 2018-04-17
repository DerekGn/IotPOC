import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { GatewayComponent } from './gateway/gateway.component';
import { NetworkComponent } from './network/network.component';
import { RadioComponent } from './radio/radio.component';

const routes: Routes = [
  { path: '', redirectTo: '/gateway', pathMatch: 'full' },
  { path: 'gateway', component: GatewayComponent },
  { path: 'network', component: NetworkComponent },
  { path: 'radio', component: RadioComponent }
];

@NgModule({
  imports: [ RouterModule.forRoot(routes) ],
  exports: [ RouterModule ]
})
export class AppRoutingModule { }
