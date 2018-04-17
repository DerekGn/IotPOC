import { Injectable } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import { of } from 'rxjs/observable/of';
import { GatewayConfig } from '../models/gatewayConfig';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable()
export class GatewayService {

  constructor(private http: HttpClient) { }

  private gatewayUrl = 'http://192.168.1.128/api/gateway';

  getGatewayConfig(): Observable<GatewayConfig> {
    return this.http.get<GatewayConfig>(this.gatewayUrl)
    .pipe(
      catchError(this.handleError<any>('getGatewayConfig'))
    );
  }

  updateGateway(gatewayConfig: GatewayConfig): Observable<any> {
    return this.http.put(this.gatewayUrl, gatewayConfig, httpOptions).pipe(
      catchError(this.handleError<any>('updateHero'))
    );
  }

  /**
  * Handle Http operation that failed.
  * Let the app continue.
  * @param operation - name of the operation that failed
  * @param result - optional value to return as the observable result
  */
  private handleError<T> (operation = 'operation', result?: T) {
    return (error: any): Observable<T> => {

      // TODO: send the error to remote logging infrastructure
      console.error(error); // log to console instead

      // TODO: better job of transforming error for user consumption
      //this.log(`${operation} failed: ${error.message}`);

      // Let the app keep running by returning an empty result.
      return of(result as T);
    };
  }
}
