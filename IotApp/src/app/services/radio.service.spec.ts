import { TestBed, inject } from '@angular/core/testing';

import { RadioService } from './radio.service';

describe('RadioService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [RadioService]
    });
  });

  it('should be created', inject([RadioService], (service: RadioService) => {
    expect(service).toBeTruthy();
  }));
});
