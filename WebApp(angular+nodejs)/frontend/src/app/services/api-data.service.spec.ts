import { TestBed } from '@angular/core/testing';

import { apidataService } from './api-data.service';

describe('apidataService', () => {
  let service: apidataService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(apidataService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
